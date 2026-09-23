#pragma once

#include <vector> 
#include <string>
#include "IRequestHandler.h"
#include "Requests.h"
#include "json.hpp"

using json = nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	static RequestInfo deserializerRequest(const std::vector<unsigned char>& buffer);
	static LoginRequest deserializerLoginRequest(const RequestInfo& info);
	static SignupRequest deserializerSignUpRequest(const RequestInfo& info);
	static CreateRoomRequest deserializerCreateRoomRequest(const RequestInfo& info);
	static JoinRoomRequest deserializerJoinRoomRequest(const RequestInfo& info);
	static GetPlayersInRoomRequest deserializerGetPlayersInRoomRequest(const RequestInfo& info);
	static LeaveRoomRequest deserializerLeaveRoomRequest(const RequestInfo& info);

	static SubmitAnswerRequest deserializerSubmitAnswerRequest(const RequestInfo& info);
};