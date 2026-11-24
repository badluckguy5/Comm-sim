#include "Message.h"

namespace {
	constexpr std::uint8_t PROTOCOL_VERSION = 1;
}

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

/*
	Package format:
	Field			Size
	Protocol Version	1 byte
	Sender ID		4 bytes
	Receiver ID		4 bytes
	Message Type		1 byte
	Priority		1 byte
	Payload Length		4 bytes
	Payload			Variable

	Integers are serialized in Big-Endian order
*/
std::vector<std::uint8_t> Message::serialize() const {
	std::vector<uint8_t> packet;

	//Protocol Version
	packet.push_back(PROTOCOL_VERSION);

	//Sender ID
	std::uint32_t senderIdValue = static_cast<std::uint32_t>(senderId_);
	for (int i = 0; i < 4; i++) {
		std::uint32_t tempByte = senderIdValue;
		tempByte >>= (24 - (8*i));
		tempByte &= 0xFF;

		packet.push_back(static_cast<std::uint8_t>(tempByte));
	}

	//Receiver ID
	std::uint32_t receiverIdValue = static_cast<std::uint32_t>(receiverId_);
	for (int i = 0; i < 4; i++) {
		std::uint32_t tempByte = receiverIdValue;
		tempByte >>= (24 - (8*i));
		tempByte &= 0xFF;

		packet.push_back(static_cast<std::uint8_t>(tempByte));
	}

/*	Message Type:
	0: Data
	1: Heartbeat
	2: HeartbeatAck
*/
	packet.push_back(static_cast<std::uint8_t>(type_));

/*	Priority:
	0: Low
	1: Normal
	2: High
	3: Urgent
*/
	packet.push_back(static_cast<std::uint8_t>(priority_));

	//Payload Length
	std::uint32_t messageLength = static_cast<std::uint32_t>(content_.size());
	for (int i = 0; i < 4; i++) {
		std::uint32_t tempByte = messageLength;
		tempByte >>= (24 - (8*i));
		tempByte &= 0xFF;

		packet.push_back(static_cast<std::uint8_t>(tempByte));
	}

	//Payload
	packet.insert(packet.end(), content_.begin(), content_.end());

	return packet;
}



