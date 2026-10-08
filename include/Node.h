#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <optional>
#include "Message.h"

struct NodeHealth
{
	bool online;
	int missedHeartbeats;
	std::chrono::steady_clock::time_point lastSuccessfulContact;
};

class Node
{
public:
	Node(int id, std::string name);
	int getId() const;
	std::string getName() const;
	std::optional<Message> receiveMessage(const Message& message);
	void start();
	void stop();
	bool isRunning() const;
	const std::vector<Message>& getMessages() const;
	void addKnownNode(int nodeId);
	const NodeHealth* getNodeHealth(int nodeId) const;
	void recordSuccessfulContact(int nodeId);
	void recordMissedHeartbeat(int nodeId);

private:
	int id_;
	std::string name_;
	bool running_;
	std::vector<Message> receivedMessages_;
	std::unordered_map<int, NodeHealth> nodeHealth_;
};

#endif
