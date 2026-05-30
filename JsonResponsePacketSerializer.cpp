#include "JsonResponsePacketSerializer.h"
#include "json.hpp"

using json = nlohmann::json;

std::vector<unsigned char> JsonResponsePacketSerializer::buildPacket(unsigned char code, const std::string& jsonMsg) {
	std::vector<unsigned char> buffer;

	buffer.push_back(code);

	unsigned int size = static_cast<unsigned int>(jsonMsg.length());
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
	return buildPacket(10, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLoginResponse(const LoginResponse& response) {
	json j;
	j["status"] = response.status;
	return buildPacket(11, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeSignupResponse(const SignupResponse& response) {
	json j;
	j["status"] = response.status;
	return buildPacket(12, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetRoomsResponse(const GetRoomsResponse& response) {
	json j;
	j["rooms"] = json::array();
	for (const auto& room : response.rooms) {
		json r;
		r["id"] = room.id;
		r["name"] = room.name;
		r["maxPlayers"] = room.maxPlayers;
		r["numOfQuestions"] = room.numOfQuestions;
		r["timePerQuestion"] = room.timePerQuestion;
		j["rooms"].push_back(r);
	}
	return buildPacket(13, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeCreateRoomResponse(const CreateRoomResponse& response) {
	json j;
	j["status"] = response.status;
	j["roomId"] = response.roomId;
	return buildPacket(14, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeJoinRoomResponse(const JoinRoomResponse& response) {
	json j;
	j["status"] = response.status;
	return buildPacket(15, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetPlayersInRoomRespponse(const GetPlayersInRoomResponse& response) {
	json j;
	j["players"] = response.players;
	return buildPacket(16, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLeaveRoomResponse(const LeaveRoomResponse& response) {
	json j;
	j["status"] = response.status;
	return buildPacket(17, j.dump());
}

//Aтттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттттт




std::vector<unsigned char> JsonResponsePacketSerializer::serializeCloseRoomResponse(const CloseRoomResponse& response) {
	json j;
	j["status"] = response.status;
	return buildPacket(18, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeStartGameResponse(const StartGameResponse& response) {
	json j;
	j["status"] = response.status;
	return buildPacket(19, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetRoomStateResponse(const GetRoomStateResponse& response) {
	json j;
	j["status"] = response.status;
	j["hasGameBegun"] = response.hasGameBegun;
	j["players"] = response.players;
	j["AnswerCount"] = response.answerCount;
	j["answerTimeOut"] = response.answerTimeout;
	return buildPacket(20, j.dump());
}