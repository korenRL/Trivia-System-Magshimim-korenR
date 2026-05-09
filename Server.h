#pragma once

#include "SqliteDataBase.h" 
#include "RequestHandlerFactory.h"
#include "Communicator.h"

class Server
{
public:
	Server();
	~Server();
	void run();

private:
	SqliteDataBase* m_database;

	RequestHandlerFactory m_handlerFactory;
	Communicator m_communicator;
};