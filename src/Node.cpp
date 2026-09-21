#include "Node.h"
#include "Message.h"
#include <iostream>

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

void Node::sendMessage(const Node& receiver, const std::string& message) const {

	Message msg(id_, message);

	std::cout << name_ << "->" << receiver.getName()
	<< ": " << message << std::endl;
}

void Node::receiveMessage(const Message& message) const{
	std::cout << name_ << " received message from Node " 
	<< message.getSenderId()
	<< ": " << message.getContent() << std::endl;
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
