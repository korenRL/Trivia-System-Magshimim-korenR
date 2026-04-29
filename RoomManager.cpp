#include "RoomManager.h"

int RoomManager::createRoom(const std::string& name, unsigned int maxPlayers, unsigned int numQuestions, unsigned int timePerQuestion)
{
	Room room;

	room.id = m_nextRoomId;
	m_nextRoomId++;

	room.name = name;
	room.maxPlayers = maxPlayers;
	room.numOfQuestions = numQuestions;
	room.timePerQuestion = timePerQuestion;

	m_rooms[room.id] = room;
	return room.id;
}

std::vector<Room> RoomManager::getRooms() const
{
	std::vector<Room> rooms;
	for (const auto& pair : m_rooms)
	{
		rooms.push_back(pair.second);
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
	if (room.players.size() >= room.maxPlayers)
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