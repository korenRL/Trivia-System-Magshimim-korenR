#pragma once
#include <string>

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxPlayers;
	unsigned int questionCount;
	unsigned int answerCooldown;
};

struct JoinRoomRequest
{
	unsigned int roomId;
};

struct getRoomsRequest
{

};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

struct LeaveRoomRequest
{
	unsigned int roomId;
};