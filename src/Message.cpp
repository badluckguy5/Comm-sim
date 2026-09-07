#include "Message.h"

Message::Message(int senderId, std::string content)
	: senderId_(senderId), content_(content)
{
}

int Message::getSenderId() const {
	return senderId_;
}

std::string Message::getContent() const {
	return content_;
}

