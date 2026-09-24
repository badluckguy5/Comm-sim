#include "Message.h"

Message::Message(int senderId, int receiverId, Priority priority, std::string content)
	: senderId_(senderId), receiverId_(receiverId), priority_(priority), content_(content)
{
}

int Message::getSenderId() const {
	return senderId_;
}

int Message::getReceiverId() const {
	return receiverId_;
}

Message::Priority Message::getPriority() const {
	return priority_;
}

std::string Message::getContent() const {
	return content_;
}

