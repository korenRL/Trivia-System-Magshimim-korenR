#include "Server.h"
#include <iostream>
#include <exception>

Server::Server()
{
	LoginManager* loginManager = new LoginManager(nullptr);
	RoomManager* roomManager = new RoomManager();
	StatisticsManager* statisticsManager = new StatisticsManager(nullptr);

	m_handlerFactory = new RequestHandlerFactory(nullptr, loginManager, roomManager, statisticsManager);

	m_communicator = new Communicator(m_handlerFactory);
}

Server::~Server()
{
	delete m_communicator;
	delete m_handlerFactory;
}

void Server::run()
{
	std::cout << "Starting server..." << std::endl;
	m_communicator->startHandleRequests();
}