#pragma once

#include <vector>
#include <string>
#include "structs.h"

class JsonResponsePacketSerializer {
public:
	static std::vector<unsigned char> serializeErrorResponse(const ErrorResponse& response);
	static std::vector<unsigned char> serializeLoginResponse(const LoginResponse& response);
	static std::vector<unsigned char> serializeSignupResponse(const SignupResponse& response);

	static std::vector<unsigned char> serializeGetRoomsResponse(const GetRoomsResponse& response);
	static std::vector<unsigned char> serializeCreateRoomResponse(const CreateRoomResponse& response);
	static std::vector<unsigned char> serializeJoinRoomResponse(const JoinRoomResponse& response);
	static std::vector<unsigned char> serializeGetPlayersInRoomRespponse(const GetPlayersInRoomResponse& response);
	static std::vector<unsigned char> serializeLeaveRoomResponse(const LeaveRoomResponse& response);

	// פונקציות חדשות לגרסה 3.0.0
	static std::vector<unsigned char> serializeCloseRoomResponse(const CloseRoomResponse& response);
	static std::vector<unsigned char> serializeStartGameResponse(const StartGameResponse& response);
	static std::vector<unsigned char> serializeGetRoomStateResponse(const GetRoomStateResponse& response);

private:
	static std::vector<unsigned char> buildPacket(unsigned char code, const std::string& jsonMsg);
};