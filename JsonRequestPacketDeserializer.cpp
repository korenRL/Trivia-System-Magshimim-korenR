#include "JsonRequestPacketDeserializer.h"

RequestInfo JsonRequestPacketDeserializer::deserializerRequest(const std::vector<unsigned char>& buffer)
{
    if (buffer.size() < 5)
        throw std::runtime_error("Invalid packet");

    RequestInfo info;
    info.messageCode = buffer[0];

    unsigned int dataSize = 0;
    dataSize |= ((unsigned int)buffer[1]);
    dataSize |= ((unsigned int)buffer[2] << 8);
    dataSize |= ((unsigned int)buffer[3] << 16);
    dataSize |= ((unsigned int)buffer[4] << 24);

    if (buffer.size() < 5 + dataSize)
        throw std::runtime_error("Incomplete packet");

    info.buff = std::vector<unsigned char>(buffer.begin() + 5, buffer.begin() + 5 + dataSize);
    info.receivalTime = std::time(NULL);

    return info;
}

LoginRequest JsonRequestPacketDeserializer::deserializerLoginRequest(const RequestInfo& info)
{
    std::string jsonStr(info.buff.begin(), info.buff.end());
    json j = json::parse(jsonStr);

    if (!j.contains("username") || !j["username"].is_string())
        throw std::runtime_error("Invalid username");

    if (!j.contains("password") || !j["password"].is_string())
        throw std::runtime_error("Invalid password");

    LoginRequest req;
    req.username = j["username"];
    req.password = j["password"];
    return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializerSignUpRequest(const RequestInfo& info)
{
    std::string jsonStr(info.buff.begin(), info.buff.end());
    json j = json::parse(jsonStr);

    if (!j.contains("username") || !j["username"].is_string())
        throw std::runtime_error("Invalid username");

    if (!j.contains("password") || !j["password"].is_string())
        throw std::runtime_error("Invalid password");

    if (!j.contains("email") || !j["email"].is_string())
        throw std::runtime_error("Invalid email");

    SignupRequest req;
    req.username = j["username"];
    req.password = j["password"];
    req.email = j["email"];
    return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializerCreateRoomRequest(const RequestInfo& info)
{
    std::string jsonStr(info.buff.begin(), info.buff.end());
    json j = json::parse(jsonStr);

    if (!j.contains("roomName") || !j.contains("maxPlayers") || !j.contains("questionCount") || !j.contains("answerCooldown"))
        throw std::runtime_error("Invalid create room request");

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

    if (!j.contains("roomId"))
        throw std::runtime_error("Invalid join room request");

    JoinRoomRequest req;
    req.roomId = j["roomId"];
    return req;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializerGetPlayersInRoomRequest(const RequestInfo& info)
{
    std::string jsonStr(info.buff.begin(), info.buff.end());
    json j = json::parse(jsonStr);

    if (!j.contains("roomId"))
        throw std::runtime_error("Invalid get players request");

    GetPlayersInRoomRequest req;
    req.roomId = j["roomId"];
    return req;
}

LeaveRoomRequest JsonRequestPacketDeserializer::deserializerLeaveRoomRequest(const RequestInfo& info)
{
    std::string jsonStr(info.buff.begin(), info.buff.end());
    json j = json::parse(jsonStr);

    if (!j.contains("roomId"))
        throw std::runtime_error("Invalid leave room request");

    LeaveRoomRequest req;
    req.roomId = j["roomId"];
    return req;
}