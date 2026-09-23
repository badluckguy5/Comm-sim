#include "Node.h"
#include "Message.h"
#include <iostream>
#include <vector>

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

void Node::receiveMessage(const Message& message) {
	std::cout << name_ << " received message from Node " 
	<< message.getSenderId() << std::endl;

	receivedMessages_.push_back(message);
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













