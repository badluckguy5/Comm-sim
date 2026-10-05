#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message {
public:
	enum class Type {
		Data,
		Heartbeat,
		HeartbeatAck,
	};

	enum class Priority {
		Low,
		Normal,
		High,
		Urgent
	};

	Message(int senderId, int receiverId, Type type);
	Message(int senderId, int receiverId, Type type, Priority priority, std::string content);

	int getSenderId() const;
	int getReceiverId() const;
	Priority getPriority() const;
	std::string getContent() const;
	Type getType() const;

private:
	int senderId_;
	int receiverId_;
	Type type_;
	Priority priority_;
	std::string content_;
};

#endif
