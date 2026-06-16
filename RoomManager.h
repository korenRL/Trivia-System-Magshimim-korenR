#pragma once

#include <vector>
#include <string>
#include <map>

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

class Room
{
public:
	RoomData metadata;
	std::vector<std::string> players;

	Room() = default;
	Room(const RoomData& data) : metadata(data) {}

	void addUser(const std::string& username);
	void removeUser(const std::string& username);
	std::vector<std::string> getAllUsers() const;

};

class RoomManager
{
private:
	std::map<unsigned int, Room> m_rooms;
	unsigned int m_nextRoomId = 1;

public:
	RoomManager() = default;

	int createRoom(
		const std::string& name,
		unsigned int maxPlayers,
		unsigned int numQuestions, 
		unsigned int timePerQuestion
	);

	std::vector<Room> getRooms() const;
	bool joinRoom(unsigned int roomId, const std::string& username);
	void leaveRoom(unsigned int roomId, const std::string& username);
	void deleteRoom(unsigned int roomId);
	unsigned int getRoomState(unsigned int roomId) const;
	Room& getRoom(unsigned int roomId);
	bool roomExists(unsigned int roomId) const;
};