#include "Network.h"
#include <iostream>
#include <random>

Network::Network()
	: lossProbability_(0.0), randomEngine_(std::random_device{}())
{
}

void Network::transmit(const Node& sender,
			Node& receiver,
			const Message& message)
{
	std::cout << sender.getName() << " -> "
	<< receiver.getName() << ": " 
	<< message.getContent() << std::endl;

	std::uniform_real_distribution<double> distrib(0.0, 1.0);

	if (distrib(randomEngine_) < lossProbability_) {
		std::cout << "Message Dropped" << std::endl;
		return;
	}

	receiver.receiveMessage(message);
	return;
}

void Network::addNode(Node* node) {
	nodes_.push_back(node);
}

Node* Network::findNode(int id) {
	for (Node* node : nodes_) {
		if (node->getId() == id) {
			return node;
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

	Message message = Message(senderId, receiverId, priority, content);

	transmit(*sender, *receiver, message);
}


void Network::setLossProbability(double probability) {
	if (probability < 0 || probability > 1) {
		std::cout << "Invalid Loss Probability" << std::endl;
		return;
	}

	lossProbability_ = probability;
	return;
}
