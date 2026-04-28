#pragma once

#include <vector>
#include <string>
#include <map>

struct Room
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestions;
	unsigned int timePerQuestion;

	std::vector<std::string> players;
};

class RoomManager
{
private:
	std::map<unsigned int, Room> m_rooms;
	unsigned int m_nextRoomId = 1;

public:
	RoomManager() = default;

	int createRoom(const std::string& name, unsigned int maxPlayers, unsigned int numQuestions, unsigned int timePerQuestion);
	std::vector<Room> getRooms() const;
	bool joinRoom(unsigned int roomId, const std::string& username);
	void leaveRoom(unsigned int roomId, const std::string& username);

};