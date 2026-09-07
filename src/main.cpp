#include <iostream>
#include "Node.h"
#include "Network.h"

int main() {
	Node node1(1, "Alpha");
	Node node2(2, "Bravo");

	Network network;

	network.addNode(node1);
	network.addNode(node2);

	Node* testNode1 = network.findNode(2);
	if (testNode1 != nullptr) {
		std::cout << "Test Node 1 Name: " << testNode1->getName() << std::endl;
	}
	else {
		std::cout << "Test Node 1 is a nullptr" << std::endl;
	}

	Node* testNode2 = network.findNode(5);
	if (testNode2 != nullptr) {
		std::cout << "Test Node 2 Name: " << testNode2->getName() << std::endl;
	}
	else {
		std::cout << "Test Node 2 is a nullptr" << std::endl;
	}

	std::cout << "Comm Simulator" << std::endl;

	std::cout << "Node " << node1.getId()
	<< ": " << node1.getName() << std::endl;

	std::cout << "Node " << node2.getId()
	<< ": " << node2.getName() << std::endl;

	Message message(node1.getId(), "Hello Bravo!");
	network.transmit(node1, node2, message);

	return 0;
}
