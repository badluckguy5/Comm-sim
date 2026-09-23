#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "Message.h"

class Node
{
public:
	Node(int id, std::string name);
	int getId() const;
	std::string getName() const;
	void receiveMessage(const Message& message);
	void start();
	void stop();
	bool isRunning() const;
	const std::vector<Message>& getMessages() const;

private:
	int id_;
	std::string name_;
	bool running_;
	std::vector<Message> receivedMessages_;
};

#endif
