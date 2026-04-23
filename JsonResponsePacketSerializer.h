#pragma once
#pragma once
#include <vector>
#include "structs.h"

class JsonResponsePacketSerializer {
public:
    static std::vector<unsigned char> serializeErrorResponse(const ErrorResponse& response);
    static std::vector<unsigned char> serializeLoginResponse(const LoginResponse& response);
    static std::vector<unsigned char> serializeSignupResponse(const SignupResponse& response);

private:

    static std::vector<unsigned char> buildPacket(unsigned char code, const std::string& jsonMsg);
};