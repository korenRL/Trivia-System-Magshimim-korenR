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
	static RequestInfo deserializerRequest(const std::vector<unsigned char>& buffer)
	{
		RequestInfo info;
		info.messageCode = buffer[0];

		unsigned int dataSize = 0;
		dataSize = ((unsigned int)buffer[1] << 24);
		dataSize = ((unsigned int)buffer[2] << 16);
		dataSize = ((unsigned int)buffer[3] << 8);
		dataSize = ((unsigned int)buffer[4] << 4);

		info.buff = std::vector<unsigned char>(buffer.begin() + 5, buffer.begin() + 5 + dataSize);
		info.receivalTime = std::time(NULL);
		
		return info;
	}

	static LoginRequest deserializerLoginRequest(const RequestInfo& info)
	{
		std::string jsonStr(info.buff.begin(), info.buff.end());
		json j = json::parse(jsonStr);

		LoginRequest req;
		req.username = j["username"];
		req.password = j["password"];
		return req;
	}

	static SignupRequest deserializerSignUpRequest(const RequestInfo& info)
	{
		std::string jsonStr(info.buff.begin(), info.buff.end());
		json j = json::parse(jsonStr);

		SignupRequest req;
		req.username = j["usernname"];
		req.password = j["password"];
		req.email = j["mail"];
		return req;
	}
};