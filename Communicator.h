
#pragma once
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <map>
#include <mutex>
#include <thread>
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

#define SERVER_PORT 8826

class Communicator
{
public:
	
	Communicator(RequestHandlerFactory& handlerFactory);
	~Communicator();

	void startHandleRequests();

private:
	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);

	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory; 
	std::mutex m_clientsMutex;
};