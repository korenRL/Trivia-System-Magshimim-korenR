#include "JsonRequestPacketDeserializer.h"

RequestInfo JsonRequestPacketDeserializer::deserializerRequest(const std::vector<unsigned char>& buffer)
{
	RequestInfo info;
	info.messageCode = buffer[0];

	unsigned int dataSize = 0;
	dataSize |= (unsigned int)buffer[1];
	dataSize |= (unsigned int)buffer[2] << 8;
	dataSize |= (unsigned int)buffer[3] << 16;
	dataSize |= (unsigned int)buffer[4] << 24;

	info.buff = std::vector<unsigned char>(buffer.begin() + 5, buffer.begin() + 5 + dataSize);
	info.receivalTime = std::time(NULL);

	return info;
}

LoginRequest JsonRequestPacketDeserializer::deserializerLoginRequest(const RequestInfo& info)
{
	std::string jsonStr(info.buff.begin(), info.buff.end());
	json j = json::parse(jsonStr);

	LoginRequest req;
	req.username = j["username"];
	req.password = j["password"];
	return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializerSignUpRequest(const RequestInfo& info)
{
	std::string jsonStr(info.buff.begin(), info.buff.end());
	json j = json::parse(jsonStr);

	SignupRequest req;
	req.username = j["username"];
	req.password = j["password"];
	req.email = j["email"];
	return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializerCreateRoomRequest(const RequestInfo& info)
{
	std::string jsonStr(info.buff.begin(), info.buff.end());;
	json j = json::parse(jsonStr);

	CreateRoomRequest req;
	req.roomName = j["roomName"];
	req.maxPlayers = j["maxPlayers"];
	req.questionCount = j["questionCount"];
	req.answerCooldown = j["answerCooldown"];
	return req;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializerJoinRoomRequest(const RequestInfo& info)
{
	std::string jsonStr(info.buff.begin(), info.buff.end());
	json j = json::parse(jsonStr);

	JoinRoomRequest req;
	req.roomId = j["roomId"];
	return req;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializerGetPlayersInRoomRequest(const RequestInfo& info)
{
	std::string jsonStr(info.buff.begin(), info.buff.end());
	json j = json::parse(jsonStr);

	GetPlayersInRoomRequest req;
	req.roomId = j["roomId"];
	return req;
}

LeaveRoomRequest JsonRequestPacketDeserializer::deserializerLeaveRoomRequest(const RequestInfo& info)
{
	std::string jsonStr(info.buff.begin(), info.buff.end());
	json j = json::parse(jsonStr);
	LeaveRoomRequest req;
	req.roomId = j["roomId"];
	return req;
}
