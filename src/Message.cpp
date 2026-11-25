#include "Message.h"
#include <limits>

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

std::optional<Message> Message::deserialize(const std::vector<std::uint8_t>& data) {
	if (data.size() < 15 || data[0] != PROTOCOL_VERSION) {
		return std::nullopt;
	}

	std::uint32_t senderIdValue = 0;
	for (std::size_t i = 1; i <= 4; i++) {
		senderIdValue <<= 8;
		senderIdValue += data[i];
	}

	std::uint32_t receiverIdValue = 0;
	for (std::size_t i = 5; i <= 8; i++) {
		receiverIdValue <<= 8;
		receiverIdValue += data[i];
	}

	if (data[9] > 2) {
		return std::nullopt;
	}
	Message::Type messageType = static_cast<Message::Type>(data[9]);

	if (data[10] > 3) {
		return std::nullopt;
	}
	Message::Priority messagePriority = static_cast<Message::Priority>(data[10]);

	std::uint32_t payloadLength = 0;
	for (std::size_t i = 11; i <= 14; i++) {
		payloadLength <<= 8;
		payloadLength += data[i];
	}
	if (static_cast<std::size_t>(payloadLength) != data.size() - 15) {
		return std::nullopt;
	}

	std::string payload(data.begin() + 15, data.end());

	if (senderIdValue > std::numeric_limits<int>::max() || receiverIdValue > std::numeric_limits<int>::max()) {
		return std::nullopt;
	}

	Message message(static_cast<int>(senderIdValue), static_cast<int>(receiverIdValue), messageType, messagePriority, payload);

	return message;
}































