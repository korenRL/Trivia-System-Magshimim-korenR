#pragma once

#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"

class Server
{
private:
	RequestHandlerFactory* m_handlerFactory;
	Communicator* m_communicator;

public:
	Server();
	~Server();

	void run();
};