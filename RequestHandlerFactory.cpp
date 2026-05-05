#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(SqliteDataBase* database,
	LoginManager* loginManager,
	RoomManager* roomManager,
	StatisticsManager* statisticsManager)
{
	m_database = database;
	m_loginManager = loginManager;
	m_roomManager = roomManager;
	m_statisticsManager = statisticsManager;
}

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(m_loginManager, this);
}

IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const std::string& username)
{
	return new MenuRequestHandler(
		m_loginManager,
		m_roomManager,
		m_statisticsManager,
		username
	);
}