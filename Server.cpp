#include "Server.h"
#include "SqliteDataBase.h" 
#include <iostream>
<<<<<<< HEAD
#include <exception>

Server::Server()
{
	LoginManager* loginManager = new LoginManager(nullptr);
	RoomManager* roomManager = new RoomManager();
	StatisticsManager* statisticsManager = new StatisticsManager(nullptr);

	m_handlerFactory = new RequestHandlerFactory(nullptr, loginManager, roomManager, statisticsManager);

	m_communicator = new Communicator(m_handlerFactory);
=======

Server::Server()
	: m_database(new SqliteDataBase("TriviaDB.sqlite")),
	m_handlerFactory(m_database), 
	m_communicator(m_handlerFactory) 
{
	
>>>>>>> origin/develop
}

Server::~Server()
{
<<<<<<< HEAD
	delete m_communicator;
	delete m_handlerFactory;
=======
	if (m_database != nullptr)
	{
		delete m_database;
		m_database = nullptr;
	}
>>>>>>> origin/develop
}

void Server::run()
{
<<<<<<< HEAD
	std::cout << "Starting server..." << std::endl;
	m_communicator->startHandleRequests();
=======
	std::cout << "Starting Server..." << std::endl;

	m_communicator.startHandleRequests();
>>>>>>> origin/develop
}