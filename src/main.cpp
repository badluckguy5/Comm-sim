#include <iostream>
#include <thread>
#include <chrono>
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
	network.setLossProbability(0.5);
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

	std::cout << "\n//Send tests" << std::endl;

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

	auto messages = node2.getMessages();

	for (int i = 0; i < messages.size(); i++) {
		std::cout << "Message " << i + 1 << ":\n\t Sender: Node " << messages[i].getSenderId() 
		<< "\n\t Receiver: Node " << messages[i].getReceiverId() 
		<< "\n\t Priority: " << priorityToString(messages[i].getPriority()) 
		<< "\n\t Content: " << messages[i].getContent() << std::endl;
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

	auto health = node1.getNodeHealth(node2.getId());

	if (health) {
		std::cout << "Node 2 health record found\nOnline: " << health->online <<
		"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl; 
	}

	else {
		std::cout << "Node 2 health record not found" << std::endl;
	}

	auto unknownHealth = node1.getNodeHealth(999);

	if (!unknownHealth) {
		std::cout << "Unknown node correctly returned std::nullopt" << std::endl;
	}

	else {
		std::cout << "Unknown node has a record!" << std::endl;
	}

	node2.addKnownNode(node1.getId());

	auto health2 = node2.getNodeHealth(node1.getId());

	node1.recordSuccessfulContact(2);
	health = node1.getNodeHealth(2);

	std::cout << "Node 1 successful contact to node 2: " <<
	health->online << "\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	std::cout << std::endl;

	Message heartbeat = Message(node1.getId(), node2.getId(), Message::Type::Heartbeat);

	network.transmit(node1, node2, heartbeat);

	health = node1.getNodeHealth(2);
	health2 = node2.getNodeHealth(1);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	std::cout << "\nNode 2 status to Node 1: " << health2->online <<
	"\nMissed Heartbeats: " << health2->missedHeartbeats << std::endl;

	std::cout << "\n/Transmission Tests" << std::endl;

	node1.stop();

	network.transmit(node1, node2, heartbeat);

	node1.start();
	node2.stop();

	network.transmit(node1, node2, heartbeat);

	node2.start();

	network.transmit(node1, node2, heartbeat);

	network.sendHeartbeat(node1.getId(), node2.getId());

	health = node1.getNodeHealth(2);

	if (health) {
		std::cout << "Alpha's heartbeat pending for Bravo: " << std::boolalpha
                << health->heartbeatPending << "\nMissed Heartbeats: " << health->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Bravo is not registered with Alpha."
              	<< std::endl;
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));

	node1.checkHeartbeatTimeout(2, std::chrono::milliseconds(2000));

	health = node1.getNodeHealth(2);

	if (health) {
		std::cout << "Alpha's heartbeat pending for Bravo: " << std::boolalpha
                << health->heartbeatPending << "\nMissed Heartbeats: " << health->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Bravo is not registered with Alpha."
              	<< std::endl;
	}

	Node node3(3, "Charlie");
	node3.start();
	network.addNode(&node3);
	node1.addKnownNode(node3.getId());

	auto health3 = node1.getNodeHealth(node3.getId());

	if (health3) {
		std::cout << "Alpha's heartbeat pending for Charlie: " << std::boolalpha
                << health3->heartbeatPending << "\nMissed Heartbeats: " << health3->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Charlie is not registered with Alpha."
              	<< std::endl;
	}

	network.sendHeartbeat(node1.getId(), node2.getId());
	network.sendHeartbeat(node1.getId(), node3.getId());
	node1.checkAllHeartbeatTimeouts(std::chrono::milliseconds(2000));

	health = node1.getNodeHealth(2);
	health3 = node1.getNodeHealth(3);

	if (health) {
		std::cout << "Alpha's heartbeat pending for Bravo: " << std::boolalpha
                << health->heartbeatPending << "\nMissed Heartbeats: " << health->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Bravo is not registered with Alpha."
              	<< std::endl;
	}

	if (health3) {
		std::cout << "Alpha's heartbeat pending for Charlie: " << std::boolalpha
                << health3->heartbeatPending << "\nMissed Heartbeats: " << health3->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Charlie is not registered with Alpha."
              	<< std::endl;
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));
	node1.checkAllHeartbeatTimeouts(std::chrono::milliseconds(2000));

	health = node1.getNodeHealth(2);
	health3 = node1.getNodeHealth(3);

	if (health) {
		std::cout << "Alpha's heartbeat pending for Bravo: " << std::boolalpha
                << health->heartbeatPending << "\nMissed Heartbeats: " << health->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Bravo is not registered with Alpha."
              	<< std::endl;
	}

	if (health3) {
		std::cout << "Alpha's heartbeat pending for Charlie: " << std::boolalpha
                << health3->heartbeatPending << "\nMissed Heartbeats: " << health3->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Charlie is not registered with Alpha."
              	<< std::endl;
	}

	network.monitorNode(1);
	std::this_thread::sleep_for(std::chrono::seconds(4));

	health = node1.getNodeHealth(2);
	health2 = node2.getNodeHealth(1);
	health3 = node1.getNodeHealth(3);

	if (health) {
		std::cout << "Alpha's heartbeat pending for Bravo: " << std::boolalpha
                << health->heartbeatPending << "\nMissed Heartbeats: " << health->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Bravo is not registered with Alpha."
              	<< std::endl;
	}

	network.monitorAllNodes();
	health = node1.getNodeHealth(2);
	health2 = node2.getNodeHealth(1);
	health3 = node1.getNodeHealth(3);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	std::cout << "\nNode 2 status to Node 1: " << health2->online <<
	"\nMissed Heartbeats: " << health2->missedHeartbeats << std::endl;

	if (health3) {
		std::cout << "Alpha's heartbeat pending for Charlie: " << std::boolalpha
                << health3->heartbeatPending << "\nMissed Heartbeats: " << health3->missedHeartbeats
		<< std::endl;
	}

	else {
    		std::cout << "Charlie is not registered with Alpha."
              	<< std::endl;
	}

	std::cout << "\n\nRunning automated monitoring" << std::endl;

	network.startMonitoring(std::chrono::milliseconds(5000));

	std::this_thread::sleep_for(std::chrono::seconds(20));

	network.stopMonitoring();

	std::cout << "\n\nStopping automated monitoring" << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(10));

	std::cout << "\n\nTesting Second Network w/ Heartbeats" << std::endl;

	{
		Network network2;
		network2.setLossProbability(0);
		network2.setLatencyRange(100, 1000);

		network2.addNode(&node1);
		network2.addNode(&node2);

		network2.startMonitoring(std::chrono::milliseconds(500));
		std::this_thread::sleep_for(std::chrono::seconds(5));

		std::cout << "\n\nStopping second network for 3 seconds" << std::endl;

		network2.stopMonitoring();
		std::this_thread::sleep_for(std::chrono::seconds(3));

		std::cout << "\n\nStarting second network again" << std::endl;

		network2.startMonitoring(std::chrono::milliseconds(500));
		std::this_thread::sleep_for(std::chrono::seconds(5));

		std::cout << "\n\nDestroying Second network without stopping thread" << std::endl;
	}

	std::cout << "\n\nPausing to see if thread destroyed correctly" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));

	health = node1.getNodeHealth(2);
	health2 = node2.getNodeHealth(1);
	health3 = node1.getNodeHealth(3);

	std::cout << "\nNode 1 status to Node 2: " << health->online <<
	"\nMissed Heartbeats: " << health->missedHeartbeats << std::endl;

	std::cout << "\nNode 2 status to Node 1: " << health2->online <<
	"\nMissed Heartbeats: " << health2->missedHeartbeats << std::endl;

	return 0;
}
