#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"
#include "Message.h"
#include <vector>

class Network {
public:
	void transmit(const Node& sender, const Node& receiver, const Message& message);
	void addNode(const Node& node);
	Node* findNode(int id);

private:
	std::vector<Node> nodes_;
};

#endif


