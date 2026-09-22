#include "Network.h"
#include <iostream>

void Network::transmit(const Node& sender,
			const Node& receiver,
			const Message& message)
{
	std::cout << sender.getName() << " -> "
	<< receiver.getName() << ": " 
	<< message.getContent() << std::endl;

	receiver.receiveMessage(message);
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

void Network::send(int senderId, int receiverId, std::string content) {
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

	Message message = Message(senderId, content);

	transmit(*sender, *receiver, message);
}


