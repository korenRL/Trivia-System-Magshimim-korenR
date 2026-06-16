#include "RoomManager.h"

int RoomManager::createRoom(
	const std::string& name,
	unsigned int maxPlayers, 
	unsigned int numQuestions,
	unsigned int timePerQuestion
)
{
	RoomData data;
	data.id = m_nextRoomId++;
	data.name = name;
	data.maxPlayers = maxPlayers;
	data.numOfQuestionsInGame = numQuestions;
	data.timePerQuestion = timePerQuestion;
	data.isActive = 0;

	m_rooms[data.id] = Room(data);
	return data.id;
}

// Only waiting rooms are shown in the 'join room' screen
std::vector<Room> RoomManager::getRooms() const
{
	std::vector<Room> rooms;

	for (const auto& pair : m_rooms)
	{
		if (pair.second.metadata.isActive == 0)
		{
			rooms.push_back(pair.second);
		}
	}

	return rooms;
}

bool RoomManager::joinRoom(unsigned int roomId, const std::string& username)
{
	if (m_rooms.find(roomId) == m_rooms.end())
	{
		return false;
	}

	auto& room = m_rooms[roomId];

	if (room.metadata.isActive == 1)
	{
		return false;
	}

	if (room.players.size() >= room.metadata.maxPlayers)
	{
		return false;
	}

	room.players.push_back(username);
	return true;
}

void RoomManager::leaveRoom(unsigned int roomId, const std::string& username)
{
	if (m_rooms.find(roomId) == m_rooms.end())
	{
		return;
	}

	auto& players = m_rooms[roomId].players;
	for (auto it = players.begin(); it != players.end(); ++it)
	{
		if (*it == username)
		{
			players.erase(it);
			break;
		}
	}
}

void Room::addUser(const std::string& username)
{
	players.push_back(username);
}

void Room::removeUser(const std::string& username)
{
	for (auto it = players.begin(); it != players.end(); ++it)
	{
		if (*it == username)
		{
			players.erase(it);
			break;
		}
	}
}

std::vector<std::string> Room::getAllUsers() const
{
	return players;
}

void RoomManager::deleteRoom(unsigned int roomId)
{
	m_rooms.erase(roomId);
}

unsigned int RoomManager::getRoomState(unsigned int roomId) const
{
	if (m_rooms.find(roomId) == m_rooms.end())
	{
		return 0;
	}

	return m_rooms.at(roomId).metadata.isActive;
}

Room& RoomManager::getRoom(unsigned int roomId)
{
	return m_rooms[roomId];
}

/*
* Used by members already inside a room, even after the
* room becomes active and is hidden from the public list.
*/
bool RoomManager::roomExists(unsigned int roomId) const
{
	return m_rooms.find(roomId) != m_rooms.end();
}