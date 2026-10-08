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
	network.setLossProbability(.5);
	network.setLatencyRange(100, 3000);

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

	std::cout << "\n//Node failure tests" << std::endl;

	std::cout << "Node 1 is: " << node1.isRunning() << std::endl;
	network.failNode(1);

	std::cout << "Node 1 is: " << node1.isRunning() << std::endl;
	network.failNode(1);

	std::cout << "Node 1 is: " << node1.isRunning() << std::endl;
	network.recoverNode(1);

	std::cout << "Node 1 is: " << node1.isRunning() << std::endl;
	network.recoverNode(1);

	std::cout << "\nHeartbeat tests" << std::endl;

	node1.addKnownNode(node2.getId());

	const NodeHealth* health = node1.getNodeHealth(node2.getId());

	if (health != nullptr) {
		std::cout << "Node 2 health record found\nOnline: " << health->online <<
		"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl; 
	}

	else {
		std::cout << "Node 2 health record not found" << std::endl;
	}

	const NodeHealth* unknownHealth = node1.getNodeHealth(999);

	if (unknownHealth == nullptr) {
		std::cout << "Unknown node correctly returned nullptr" << std::endl;
	}

	else {
		std::cout << "Unknown node has a record!" << std::endl;
	}

	node2.addKnownNode(node1.getId());

	const NodeHealth* health2 = node2.getNodeHealth(node1.getId());

	node1.recordSuccessfulContact(2);

	std::cout << "Node 1 successful contact to node 2: " <<
	health->online << "\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	node1.recordMissedHeartbeat(2);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	node1.recordMissedHeartbeat(2);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	node1.recordMissedHeartbeat(2);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	node1.recordMissedHeartbeat(2);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	node1.recordSuccessfulContact(2);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	std::cout << std::endl;

	Message heartbeat = Message(node1.getId(), node2.getId(), Message::Type::Heartbeat);

	network.transmit(node1, node2, heartbeat);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	std::cout << "\nNode 2 status to Node 1: " << health2->online <<
	"\nMissed Heartbeats: " << health2->missedHeartbeats << std::endl;

	return 0;
}















