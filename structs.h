#pragma once
#include <string>
#include "RoomManager.h"

enum ResponseCode : unsigned char {
    ERROR_RES = 10,
    LOGIN_RES = 11,
    SIGNUP_RES = 12
};

struct ErrorResponse {
    std::string message;
};

struct LoginResponse {
    unsigned int status;
};

struct SignupResponse {
    unsigned int status;
};

struct GetRoomsResponse {
    std::vector<Room> rooms;
};

struct CreateRoomResponse {
    unsigned int status;
    unsigned int roomId;
};

struct JoinRoomResponse {
    unsigned int status;
};

struct GetPlayersInRoomResponse {
    std::vector<std::string> players;
};

struct LeaveRoomResponse
{
    unsigned int status;
};