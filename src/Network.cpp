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

void Network::addNode(const Node& node) {
	nodes_.push_back(node);
}

Node* Network::findNode(int id) {
	for (Node& node : nodes_) {
		if (node.getId() == id) {
			return &node;
		}
	}
	return nullptr;
}
