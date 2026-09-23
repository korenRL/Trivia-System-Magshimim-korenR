
#include "Server.h"
#include "SqliteDataBase.h" 
#include <iostream>
#include <exception>
#include <ctime>

Server::Server()
	: m_database(new SqliteDataBase("TriviaDB.sqlite")),
	m_handlerFactory(m_database),
	m_communicator(m_handlerFactory)
{
	srand((unsigned int)time(NULL));
}

Server::~Server()
{
	if (m_database != nullptr)
	{
		delete m_database;
		m_database = nullptr;
	}
}

void Server::run()
{
	std::cout << "Starting Server..." << std::endl;

	m_communicator.startHandleRequests();
}