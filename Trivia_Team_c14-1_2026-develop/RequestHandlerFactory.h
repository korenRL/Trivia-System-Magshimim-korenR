
#pragma once

#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "SqliteDataBase.h"
#include "LoginManager.h"
#include <string>
#include "GameManager.h"

class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class GameRequestHandler;

class RequestHandlerFactory
{
private:
	SqliteDataBase* m_database;
	LoginManager* m_loginManager;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
	GameManager* m_gameManager;

public:
	RequestHandlerFactory(SqliteDataBase* database);
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();

	IRequestHandler* createMenuRequestHandler(const std::string& username);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, Room room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser user, Room room);
	GameRequestHandler* createGameRequestHandler(LoggedUser user, unsigned int gameId);
	GameManager* getGameManager();

	RoomManager* getRoomManager();
	StatisticsManager* getStatisticsManager();
};