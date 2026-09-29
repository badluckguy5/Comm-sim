#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"
#include "Message.h"
#include <vector>
#include <string>
#include <random>

class Network {
public:
	Network();
	void transmit(const Node& sender, Node& receiver, const Message& message);
	void addNode(Node* node);
	Node* findNode(int id);
	void send(int senderId, int receiverId, Message::Priority priority, std::string content);
	void setLossProbability(double probability);

private:
	std::vector<Node*> nodes_;
	double lossProbability_;
	std::mt19937 randomEngine_;
};

#endif


