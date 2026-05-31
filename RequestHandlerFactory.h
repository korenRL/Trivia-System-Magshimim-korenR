#pragma once

#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "SqliteDataBase.h"
#include "LoginManager.h"
#include <string>

class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

class RequestHandlerFactory
{
private:
	SqliteDataBase* m_database;
	LoginManager* m_loginManager;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;

public:
<<<<<<< HEAD
	RequestHandlerFactory(SqliteDataBase* database,
		LoginManager* loginManager,
		RoomManager* roomManager,
		StatisticsManager* statisticsManager);

	IRequestHandler* createLoginRequestHandler();
	IRequestHandler* createMenuRequestHandler(const std::string& username);

	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, Room room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser user, Room room);

	RoomManager* getRoomManager();
	StatisticsManager* getStatisticsManager();
=======
	RequestHandlerFactory(SqliteDataBase* database);
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();

private:
	SqliteDataBase* m_database;
	LoginManager* m_loginManager;
	StatisticsManager* m_statisticsManager;
>>>>>>> origin/develop
};