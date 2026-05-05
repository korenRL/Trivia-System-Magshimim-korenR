#pragma once

#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "SqliteDataBase.h"
#include <string>

class RequestHandlerFactory
{
private:
	SqliteDataBase* m_database;
	LoginManager* m_loginManager;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;

public:
	RequestHandlerFactory(SqliteDataBase* database,
		LoginManager* loginManager,
		RoomManager* roomManager,
		StatisticsManager* statisticsManager);

	IRequestHandler* createLoginRequestHandler();
	IRequestHandler* createMenuRequestHandler(const std::string& username);
};