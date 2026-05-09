#pragma once

#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "LoginManager.h"
#include "SqliteDataBase.h"
#include "StatisticsManager.h"

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(SqliteDataBase* database);
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();

private:
	SqliteDataBase* m_database;
	LoginManager* m_loginManager;
	StatisticsManager* m_statisticsManager;
};