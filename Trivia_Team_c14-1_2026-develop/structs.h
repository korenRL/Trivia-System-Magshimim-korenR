#pragma once
#include <string>
#include <vector>
#include "RoomManager.h"
#include <utility>
#include <ctime>
#include <map>

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
    GET_ROOM_STATE_RES = 20,
    HIGH_SCORES_RES = 25,
    PERSONAL_STATS_RES = 26,
    LEAVE_GAME_RES = 35,
    GET_QUESTION_RES = 36,
    SUBMIT_ANSWER_RES = 37,
    GET_GAME_RESULTS_RES = 38
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

struct PlayerStatistics
{
    unsigned int gamesPlayed;
    unsigned int correctAnswers;
    unsigned int wrongAnswers;
    float avgAnswerTime;
};

struct GameData
{
    Question currentQuestion;
    unsigned int correctAnswerCount;
    unsigned int wrongAnswerCount;
    float averageAnswerTime;
    unsigned int questionIndex;
    time_t lastQuestionTime;
    bool statsSaved;
};

struct StatisticsResponse
{
    std::vector<std::string> statistics;
};

struct PlayerResults
{
    std::string username;
    unsigned int correctAnswerCount;
    unsigned int wrongAnswerCount;
    float averageAnswerTime;
};

struct GetQuestionResponse
{
    unsigned int status;
    std::string question;
    std::map<unsigned int, std::string> answers;
};

struct SubmitAnswerResponse
{
    unsigned int status;
    unsigned int correctAnswerId;
};

struct GetGameResultsResponse
{
    unsigned int status;
    std::vector<PlayerResults> results;
};

struct LeaveGameResponse
{
    unsigned int status;
};