#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <optional>
#include <atomic>
#include <mutex>
#include "Message.h"

struct NodeHealth
{
	bool online;
	int missedHeartbeats;
	std::chrono::steady_clock::time_point lastSuccessfulContact;
	bool heartbeatPending;
	std::chrono::steady_clock::time_point heartbeatSentAt;
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
	std::vector<Message> getMessages() const;
	void addKnownNode(int nodeId);
	std::optional<NodeHealth> getNodeHealth(int nodeId) const;
	void recordSuccessfulContact(int nodeId);
	void updateMissedHeartbeat(NodeHealth& health);
	bool recordHeartbeatSent(int nodeId);
	bool checkHeartbeatTimeout(int nodeId, std::chrono::milliseconds timeout);
	void checkAllHeartbeatTimeouts(std::chrono::milliseconds timeout);
	std::vector<int> getKnownIds() const;

private:
	int id_;
	std::string name_;
	std::atomic<bool> running_;
	std::vector<Message> receivedMessages_;
	std::unordered_map<int, NodeHealth> nodeHealth_;
	mutable std::mutex nodeMutex_;
};

#endif
