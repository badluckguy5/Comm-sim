#include "Message.h"

Message::Message(int senderId, int receiverId, Type type)
	: senderId_(senderId), receiverId_(receiverId), type_(type), priority_(Priority::Normal), content_("")
{
}

Message::Message(int senderId, int receiverId, Type type, Priority priority, std::string content)
	: senderId_(senderId), receiverId_(receiverId), type_(type), priority_(priority), content_(content)
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

Message::Type Message::getType() const {
	return type_;
}
