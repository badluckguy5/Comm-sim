#include "Node.h"
#include "Message.h"
#include <iostream>
#include <chrono>
#include <optional>

Node::Node(int id, std::string name) 
	:id_(id), name_(name), running_(false)
{
}


int Node::getId() const {
	return id_;
}

std::string Node::getName() const {
	return name_;
}

std::optional<Message> Node::receiveMessage(const Message& message) {
	std::cout << name_ << " received message from Node " 
	<< message.getSenderId() << std::endl;

	receivedMessages_.push_back(message);

	switch(message.getType()) {
		case Message::Type::Data:
			return std::nullopt;

		case Message::Type::Heartbeat: {
			recordSuccessfulContact(message.getSenderId());
			return Message(id_, message.getSenderId(), Message::Type::HeartbeatAck);
		}

		case Message::Type::HeartbeatAck: {
			recordSuccessfulContact(message.getSenderId());

			auto it = nodeHealth_.find(message.getSenderId());

			if (it != nodeHealth_.end()) {
				it->second.heartbeatPending = false;
			}
			return std::nullopt;
		}
	}

	return std::nullopt;
}

void Node::start() {
	running_ = true;
}

void Node::stop() {
	running_ = false;
}

bool Node::isRunning() const {
	return running_;
}

const std::vector<Message>& Node::getMessages() const {
	return receivedMessages_;
}

void Node::addKnownNode(int nodeId) {
	if (nodeHealth_.find(nodeId) != nodeHealth_.end()) {
		return;
	}

	nodeHealth_.emplace(nodeId, NodeHealth{false, 0, std::chrono::steady_clock::now(), 
		false, std::chrono::steady_clock::now()});
}

const NodeHealth* Node::getNodeHealth(int nodeId) const {
	auto it = nodeHealth_.find(nodeId);

	if (it != nodeHealth_.end()) {
		return &it->second;
	}

	return nullptr;
}

void Node::recordSuccessfulContact(int nodeId) {
	auto it = nodeHealth_.find(nodeId);

	if (it != nodeHealth_.end()) {
		it->second.online = true;
		it->second.missedHeartbeats = 0;
		it->second.lastSuccessfulContact = std::chrono::steady_clock::now();
	}
}

void Node::recordMissedHeartbeat(int nodeId) {
	auto it = nodeHealth_.find(nodeId);

	if (it != nodeHealth_.end()) {
		it->second.missedHeartbeats += 1;

		if (it->second.missedHeartbeats >= 4) {
			it->second.online = false;
		}
	}
}

bool Node::recordHeartbeatSent(int nodeId) {
	auto it = nodeHealth_.find(nodeId);

	if (it != nodeHealth_.end()) {
		if (!it->second.heartbeatPending) {
			it->second.heartbeatPending = true;
			it->second.heartbeatSentAt = std::chrono::steady_clock::now();
			return true;
		}
	}

	return false;
}

bool Node::checkHeartbeatTimeout(int nodeId, std::chrono::milliseconds timeout) {
	auto it = nodeHealth_.find(nodeId);

	if (it != nodeHealth_.end()) {
		if (it->second.heartbeatPending && std::chrono::steady_clock::now() - it->second.heartbeatSentAt > timeout) {
			recordMissedHeartbeat(nodeId);
			it->second.heartbeatPending = false;
			return true;
		}
	}

	return false;
}
