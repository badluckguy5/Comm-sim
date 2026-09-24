#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"
#include "Message.h"
#include <vector>
#include <string>

class Network {
public:
	void transmit(const Node& sender, Node& receiver, const Message& message);
	void addNode(Node* node);
	Node* findNode(int id);
	void send(int senderId, int receiverId, Message::Priority priority, std::string content);

private:
	std::vector<Node*> nodes_;
};

#endif


