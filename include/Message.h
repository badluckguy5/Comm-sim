#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message {
public: 
	enum class Priority {
		Low,
		Normal,
		High,
		Urgent
	};

	Message(int senderId, int receiverId, Priority priority, std::string content);

	int getSenderId() const;
	int getReceiverId() const;
	Priority getPriority() const;
	std::string getContent() const;

private:
	int senderId_;
	int receiverId_;
	Priority priority_;
	std::string content_;
};

#endif
