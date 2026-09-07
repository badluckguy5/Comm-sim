#ifndef NODE_H
#define NODE_H

#include <string>

class Message;

class Node
{
public:
	Node(int id, std::string name);
	int getId() const;
	std::string getName() const;
	void sendMessage(const Node& receiver, const std::string& message) const;
	void receiveMessage(const Message& message) const;

private:
	int id_;
	std::string name_;
};

#endif
