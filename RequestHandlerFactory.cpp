

#include "RequestHandlerFactory.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(SqliteDataBase* database)
	: m_database(database),
	m_loginManager(new LoginManager(database)),
	m_roomManager(new RoomManager()),
	m_statisticsManager(new StatisticsManager(database))
{
}

RequestHandlerFactory::~RequestHandlerFactory()
{
	if (m_loginManager != nullptr)
	{
		delete m_loginManager;
		m_loginManager = nullptr;
	}

	if (m_statisticsManager != nullptr)
	{
		delete m_statisticsManager;
		m_statisticsManager = nullptr;
	}

	if (m_roomManager != nullptr)
	{
		delete m_roomManager;
		m_roomManager = nullptr;
	}
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
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

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room room)
{
	return new RoomAdminRequestHandler(room, user, *m_roomManager, *this);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room room)
{
	return new RoomMemberRequestHandler(room, user, *m_roomManager, *this);
}

RoomManager* RequestHandlerFactory::getRoomManager()
{
	return m_roomManager;
}

StatisticsManager* RequestHandlerFactory::getStatisticsManager()
{
	return m_statisticsManager;
}