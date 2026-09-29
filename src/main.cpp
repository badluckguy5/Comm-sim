#include <iostream>
#include "Node.h"
#include "Network.h"
#include "Message.h"

std::string priorityToString(Message::Priority priority) {
	switch (priority) {
		case Message::Priority::Low:
			return "Low";
		case Message::Priority::Normal:
			return "Normal";
		case Message::Priority::High:
			return "High";
		case Message::Priority::Urgent:
			return "Urgent";
	}

	return "";
}

int main() {
	std::cout << "//Basic Setup" << std::endl;

	Node node1(1, "Alpha");
	Node node2(2, "Bravo");

	node1.start();
	node2.start();

	Network network;
	network.setLossProbability(0.25);

	network.addNode(&node1);
	network.addNode(&node2);

	std::cout << "\n//Node Identification tests" << std::endl;

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

	std::cout << "\n//Transmission tests" << std::endl;

	network.send(node1.getId(), node2.getId(), Message::Priority::Normal, "Hello Bravo!");
	network.send(node1.getId(), node2.getId(), Message::Priority::High, "Second Message");
	network.send(node1.getId(), 5, Message::Priority::Normal, "Test Case 1");
	network.send(5, node2.getId(), Message::Priority::Normal, "Test Case 2");

	node1.stop();
	network.send(node1.getId(), node2.getId(), Message::Priority::Normal, "Sender Off");
	node1.start();
	node2.stop();
	network.send(node1.getId(), node2.getId(), Message::Priority::Normal,"Receiver Off");

	std::cout << "\n//Message History" << std::endl;

	std::cout << "Node 2 received " << node2.getMessages().size() << " messages" << std::endl;

	for (int i = 0; i < node2.getMessages().size(); i++) {
		std::cout << "Message " << i + 1 << ":\n\t Sender: Node " << node2.getMessages()[i].getSenderId() 
		<< "\n\t Receiver: Node " << node2.getMessages()[i].getReceiverId() 
		<< "\n\t Priority: " << priorityToString(node2.getMessages()[i].getPriority()) 
		<< "\n\t Content: " << node2.getMessages()[i].getContent() << std::endl;
	}

	return 0;
}


