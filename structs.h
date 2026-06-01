#pragma once
#include <string>
#include <vector>
#include "RoomManager.h"

enum ResponseCode : unsigned char {
    ERROR_RES = 10,
    LOGIN_RES = 11,
    SIGNUP_RES = 12,
    GET_ROOMS_RES = 13,
    CREATE_ROOM_RES = 14,
    JOIN_ROOM_RES = 15,
    GET_PLAYERS_IN_ROOM_RES = 16,
    LEAVE_ROM_RES = 17,
    CLOSE_ROOM_RES = 18,
    START_GAME_RES = 19,
    GET_ROOM_STATE_RES = 20
};

enum RequestCode : unsigned char {
    CLOSE_ROOM_REQ = 21,
    START_GAME_REQ = 22,
    GET_ROOM_STATE_REQ = 23,
    LEAVE_ROOM_REQ = 24,
    LEAVE_GAME_REQ = 31,
    GET_QUESTION_REQ = 32,
    SUBMIT_ANSWER_REQ = 33,
    GET_GAME_RESULT_REQ = 34
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
    std::vector<RoomData> rooms;
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


struct CloseRoomResponse
{
    unsigned int status;
};

struct StartGameResponse
{
    unsigned int status;
};

struct GetRoomStateResponse
{
    unsigned int status;
    bool hasGameBegun;
    std::vector<std::string> players;
    unsigned int answerCount;
    unsigned int answerTimeout;
};


struct Question
{
    std::string question;
    std::vector<std::string> possibleAnswers;
    unsigned int correctAnswerId;
};