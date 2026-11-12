#include "Network.h"
#include <iostream>
#include <random>
#include <thread>
#include <optional>

Network::Network()
	: lossProbability_(0.0), minLatency_(1), maxLatency_(3000), randomEngine_(std::random_device{}()), monitoring_(false), monitoringThread_()
{
}

Network::~Network() {
	stopMonitoring();
}

void Network::transmit(Node& sender,
			Node& receiver,
			const Message& message)
{
	std::cout << sender.getName() << " -> "
	<< receiver.getName() << ": " 
	<< message.getContent() << std::endl;

	if (!sender.isRunning()) {
		std::cout << "Sender is not running. Message cancelled" << std::endl;
		return;
	}

	if (!receiver.isRunning()) {
		std::cout << "Receiver is not running. Message cancelled" << std::endl;
		return;
	}

	double lossProbabilitySnapshot;
	int minLatencySnapshot;
	int maxLatencySnapshot;

	{
		std::lock_guard<std::mutex> lock(configMutex_);

		lossProbabilitySnapshot = lossProbability_;
		minLatencySnapshot = minLatency_;
		maxLatencySnapshot = maxLatency_;
	}

	std::uniform_real_distribution<double> distrib(0.0, 1.0);

	double lossChance;

	{
		std::lock_guard<std::mutex> lock(randomMutex_);

		lossChance = distrib(randomEngine_);
	}

	if (lossChance < lossProbabilitySnapshot) {
		std::cout << "Message Dropped" << std::endl;
		return;
	}

	std::uniform_int_distribution<int> latencyDistrib(minLatencySnapshot, maxLatencySnapshot);

	int latency;

	{
		std::lock_guard<std::mutex> lock(randomMutex_);

		latency = latencyDistrib(randomEngine_);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(latency));

	auto response = receiver.receiveMessage(message);

	if (response != std::nullopt) {
		transmit(receiver, sender, *response);
	}
}

void Network::addNode(Node* node) {
	{
		std::lock_guard<std::mutex> lock(nodesMutex_);
		nodes_.push_back(node);
	}
}

Node* Network::findNode(int id) {
	{
		std::lock_guard<std::mutex> lock(nodesMutex_);
		for (Node* node : nodes_) {
			if (node->getId() == id) {
				return node;
			}
		}
	}
	return nullptr;
}

void Network::send(int senderId, int receiverId, Message::Priority priority, std::string content) {
	Node* sender = findNode(senderId);
	Node* receiver = findNode(receiverId);

	if (sender == nullptr) {
		std::cout << "Invalid Sender ID" << std::endl;
		return;
	}

	if (receiver == nullptr) {
		std::cout << "Invalid Receiver ID" << std::endl;
		return;
	}

	if (!sender->isRunning()) {
		std::cout << "Sender is not running" << std::endl;
		return;
	}

	if (!receiver->isRunning()) {
		std::cout << "Receiver is not running" << std::endl;
		return;
	}

	Message message = Message(senderId, receiverId, Message::Type::Data, priority, content);

	transmit(*sender, *receiver, message);
}


void Network::setLossProbability(double probability) {
	if (probability < 0 || probability > 1) {
		std::cout << "Invalid Loss Probability" << std::endl;
		return;
	}

	std::lock_guard<std::mutex> lock(configMutex_);

	lossProbability_ = probability;
	return;
}

void Network::setLatencyRange(int minLatency, int maxLatency) {
	if (minLatency <= 0) {
		std::cout << "Latency must be greater than 0" << std::endl;
		return;
	}
	if (minLatency > maxLatency) {
		std::cout << "Latency min must be less than or equal to max" << std::endl;
		return;
	}
	if (maxLatency > 3000) {
		std::cout << "Latency max must be less than or equal to 3000" << std:: endl;
		return;
	}

	std::lock_guard<std::mutex> lock(configMutex_);

	minLatency_ = minLatency;
	maxLatency_ = maxLatency;
	return;
}

void Network::failNode(int id) {
	Node* node = findNode(id);
	if (node == nullptr) {
		std::cout << "Invalid Node ID" << std::endl;
		return;
	}

	if (!node->isRunning()) {
		std::cout << "Node is already down" << std::endl;
		return;
	}

	node->stop();
}

void Network::recoverNode(int id) {
	Node* node = findNode(id);
	if (node == nullptr) {
		std::cout << "Invalid Node ID" << std::endl;
		return;
	}

	if (node->isRunning()) {
		std::cout << "Node is already running" << std::endl;
		return;
	}

	node->start();
}

void Network::sendHeartbeat(int senderId, int receiverId) {
	Node* sender = findNode(senderId);
	Node* receiver = findNode(receiverId);

	if (sender == nullptr) {
		std::cout << "Invalid Sender ID" << std::endl;
		return;
	}

	if (receiver == nullptr) {
		std::cout << "Invalid Receiver ID" << std::endl;
		return;
	}

	if (!sender->isRunning()) {
		std::cout << "Sender is not running" << std::endl;
		return;
	}

	if (!receiver->isRunning()) {
		std::cout << "Receiver is not running" << std::endl;
		return;
	}

	Message message = Message(senderId, receiverId, Message::Type::Heartbeat);

	if (sender->recordHeartbeatSent(receiverId)) {
		transmit(*sender, *receiver, message);
	}
}

void Network::monitorNode(int nodeId) {
	Node* node = findNode(nodeId);

	if (node != nullptr) {
		node->checkAllHeartbeatTimeouts(std::chrono::milliseconds(3000));

		std::vector<int> knownIds = node->getKnownIds();

		for (int id : knownIds) {
			auto it = node->getNodeHealth(id);
			if (it) {
				if (!it->heartbeatPending) {
					sendHeartbeat(nodeId, id);
				}
			}
		}
	}
}

void Network::monitorAllNodes() {
	std::vector<Node*> nodeSnapshot;

	{
		std::lock_guard<std::mutex> lock(nodesMutex_);

		for (auto node : nodes_) {
			nodeSnapshot.push_back(node);
		}
	}

	for (auto* node : nodeSnapshot) {
		int id = node->getId();
		monitorNode(id);
	}
}

void Network::startMonitoring(std::chrono::milliseconds interval) {
	monitoring_ = true;

	if (!monitoringThread_.joinable()) {
		monitoringThread_ = std::thread(&Network::runMonitoring, this, interval);
	}
}

void Network::stopMonitoring() {
	monitoring_ = false;

	if (monitoringThread_.joinable()) {
		monitoringThread_.join();
	}
}

void Network::runMonitoring(std::chrono::milliseconds interval) {
	while (monitoring_) {
		monitorAllNodes();
		std::this_thread::sleep_for(interval);
	}
}








