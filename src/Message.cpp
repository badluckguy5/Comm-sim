#include "Message.h"

Message::Message(int senderId, int receiverId, std::string content)
	: senderId_(senderId), receiverId_(receiverId), content_(content)
{
}

int Message::getSenderId() const {
	return senderId_;
}

int Message::getReceiverId() const {
	return receiverId_;
}

std::string Message::getContent() const {
	return content_;
}

