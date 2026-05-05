#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <string>
#include <thread>
#include <mutex>

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

#define SERVER_PORT 8826
#define HELLO_LENGTH 5

class Communicator
{
public:
	Communicator(RequestHandlerFactory* handlerFactory);
	~Communicator();

	void startHandleRequests();

private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	std::mutex m_clientsMutex;
	RequestHandlerFactory* m_handlerFactory;

	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);
};