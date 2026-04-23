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
};