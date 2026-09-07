#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message {
public: 
	Message(int senderId, std::string content);

	int getSenderId() const;
	std::string getContent() const;

private:
	int senderId_;
	std::string content_;
};

#endif
