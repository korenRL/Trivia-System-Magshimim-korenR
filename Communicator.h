#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <string>
#include <thread>
#include <mutex>

#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "SqliteDataBase.h"
#include "LoginManager.h"
#include "RoomManager.h"

#define SERVER_PORT 8826
#define HELLO_LENGTH 5

class Communicator
{
public:
	Communicator();
	~Communicator();

	void startHandleRequests();

private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	std::mutex m_clientsMutex;

	void bindAndListen();
	void handleNewClient(SOCKET clientSocker);

	SqliteDataBase* m_database;
	LoginManager* m_loginManager;
	RoomManager* m_roomManager;
};