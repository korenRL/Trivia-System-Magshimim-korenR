#include "JsonResponsePacketSerializer.h"
#include "json.hpp"

using json = nlohmann::json;

// #define ERROR_RES 1
// #define LOGIN_RES 2
// #define SIGNUP_RES 3

std::vector<unsigned char> JsonResponsePacketSerializer::buildPacket(unsigned char code, const std::string& jsonMsg) {
	std::vector<unsigned char> buffer;

	buffer.push_back(code);

	unsigned int size = jsonMsg.length();
	buffer.push_back(size & 0xFF);
	buffer.push_back((size >> 8) & 0xFF);
	buffer.push_back((size >> 16) & 0xFF);
	buffer.push_back((size >> 24) & 0xFF);

	for (char c : jsonMsg) {
		buffer.push_back(static_cast<unsigned char>(c));
	}

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeErrorResponse(const ErrorResponse& response) {
	json j;
	j["message"] = response.message;
	std::string jsonStr = j.dump();
	return buildPacket(1, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLoginResponse(const LoginResponse& response) {
	json j;
	j["status"] = response.status;
	std::string jsonStr = j.dump();
	return buildPacket(2, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeSignupResponse(const SignupResponse& response) {
	json j;
	j["status"] = response.status;
	std::string jsonStr = j.dump();
	return buildPacket(3, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetRoomsResponse(const GetRoomsResponse& response)
{
	json j;
	j["rooms"] = json::array();
	for (const auto& room : response.rooms)
	{
		json r;
		r["id"] = room.id;
		r["name"] = room.name;
		r["maxPlayers"] = room.maxPlayers;
		r["numOfQuestions"] = room.numOfQuestions;
		r["timePerQuestion"] = room.timePerQuestion;
		j["rooms"].push_back(r);
	}

	std::string jsonStr = j.dump();
	return buildPacket(13, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeCreateRoomResponse(const CreateRoomResponse& response)
{
	json j;
	j["status"] = response.status;
	j["roomId"] = response.roomId;
	std::string jsonStr = j.dump();
	return buildPacket(14, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeJoinRoomResponse(const JoinRoomResponse& response)
{
	json j;
	j["status"] = response.status;
	std::string jsonStr = j.dump();
	return buildPacket(15, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetPlayersInRoomRespponse(const GetPlayersInRoomResponse& response)
{
	json j;
	j["players"] = response.players;
	std::string jsonStr = j.dump();
	return buildPacket(16, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLeaveRoomResponse(const LeaveRoomResponse& response)
{
	json j;
	j["status"] = response.status;
	std::string jsonStr = j.dump();
	return buildPacket(17, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeCloseRoomResponse(const CloseRoomResponse& response)
{
	json j;
	j["status"] = response.status;
	std::string jsonStr = j.dump();
	return buildPacket(18, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeStartGameResponse(const StartGameResponse& response)
{
	json j;
	j["status"] = response.status;
	std::string jsonStr = j.dump();
	return buildPacket(19, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetRoomStateResponse(const GetRoomStateResponse& response)
{
	json j;
	j["status"] = response.status;
	j["hasGameBegun"] = response.hasGameBegun;
	j["players"] = response.players;
	j["AnswerCount"] = response.answerCount;
	j["answerTimeOut"] = response.answerTimeout;

	std::string jsonStr = j.dump();
	return buildPacket(20, jsonStr);
}