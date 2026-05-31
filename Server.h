#pragma once

#include "SqliteDataBase.h" 
#include "RequestHandlerFactory.h"
#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"

class Server
{
<<<<<<< HEAD
private:
	RequestHandlerFactory* m_handlerFactory;
	Communicator* m_communicator;

public:
	Server();
	~Server();

	void run();
=======
public:
	Server();
	~Server();
	void run();

private:
	SqliteDataBase* m_database;

	RequestHandlerFactory m_handlerFactory;
	Communicator m_communicator;
>>>>>>> origin/develop
};