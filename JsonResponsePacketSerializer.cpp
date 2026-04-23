#include "JsonResponsePacketSerializer.h"
#include "json.hpp"

using json = nlohmann::json;

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

    return buildPacket(ResponseCode::ERROR_RES, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLoginResponse(const LoginResponse& response) {
    json j;
    j["status"] = response.status;

    std::string jsonStr = j.dump();
    return buildPacket(ResponseCode::LOGIN_RES, jsonStr);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeSignupResponse(const SignupResponse& response) {
    json j;
    j["status"] = response.status;

    std::string jsonStr = j.dump();
    return buildPacket(ResponseCode::SIGNUP_RES, jsonStr);
}