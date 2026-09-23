#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message {
public: 
	Message(int senderId, int receiverId, std::string content);

	int getSenderId() const;
	int getReceiverId() const;
	std::string getContent() const;

private:
	int senderId_;
	int receiverId_;
	std::string content_;
};

#endif
