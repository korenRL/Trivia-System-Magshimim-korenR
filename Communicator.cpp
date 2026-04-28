#include "Communicator.h"
#include <iostream>
#include <string>

Communicator::Communicator()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
	}

	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
	{
		throw std::runtime_error("Error creating socket: " + std::to_string(WSAGetLastError()));
	}

	m_database = new SqliteDataBase("TriviaDB.sqlite");
	m_loginManager = new LoginManager(m_database);
}

Communicator::~Communicator()
{
	if (m_serverSocket != INVALID_SOCKET)
	{
		closesocket(m_serverSocket);
	}

	for (auto& pair : m_clients)
	{
		closesocket(pair.first);
		delete pair.second;
	}

	delete m_loginManager;
	delete m_database;

	m_clients.clear();
	WSACleanup();
}

void Communicator::bindAndListen()
{
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	int bindResult = bind(m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (bindResult == SOCKET_ERROR)
	{
		throw std::runtime_error("Bind failed with error: " + std::to_string(WSAGetLastError()));
	}

	int listenResult = listen(m_serverSocket, SOMAXCONN);
	if (listenResult == SOCKET_ERROR)
	{
		throw std::runtime_error("Listen failed with error: " + std::to_string(WSAGetLastError()));
	}

	std::cout << "Listening on port " << SERVER_PORT << "..." << std::endl;
}

void Communicator::startHandleRequests()
{
	bindAndListen();

	while (true)
	{
		struct sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);

		SOCKET clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
			continue;
		}

		std::cout << "Client connected." << std::endl;

		{
			std::lock_guard<std::mutex> lock(m_clientsMutex);
			m_clients[clientSocket] = new LoginRequestHandler(m_loginManager);
		}

		std::thread t(&Communicator::handleNewClient, this, clientSocket);
		t.detach();
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	try 
	{
		std::string hello = "Hello";
		send(clientSocket, hello.c_str(), HELLO_LENGTH, 0);

		char buffer[HELLO_LENGTH + 1] = { 0 };
		recv(clientSocket, buffer, HELLO_LENGTH, 0);

		std::cout << "Client connected and handshake done." << std::endl;

		while (true)
		{
			unsigned char header[5] = { 0 };
			int bytesRecevied = recv(clientSocket, (char*)header, 5, 0);

			if (bytesRecevied <= 0)
			{
				std::cout << "Client disconnected." << std::endl;
				break;
			}

			RequestInfo requestInfo;
			requestInfo.messageCode = header[0];

			unsigned int dataSize = 0;
			dataSize |= (unsigned int)header[1] << 24;
			dataSize |= (unsigned int)header[2] << 16;
			dataSize |= (unsigned int)header[3] << 8;
			dataSize |= (unsigned int)header[4];

			std::vector<unsigned char> bufferVec(dataSize);
			if (dataSize > 0)
			{
				recv(clientSocket, (char*)bufferVec.data(), dataSize, 0);
			}

			requestInfo.buff = bufferVec;
			requestInfo.receivalTime = std::time(NULL);

			IRequestHandler* handler = nullptr;
			{
				std::lock_guard<std::mutex> lock(m_clientsMutex);
				if (m_clients.find(clientSocket) != m_clients.end())
				{
					handler = m_clients[clientSocket];
				}
			}

			if (handler && handler->isRequestRelevant(requestInfo))
			{
				RequestResult result = handler->handleRequest(requestInfo);


				if (!result.response.empty())
				{
					send(clientSocket, (char*)result.response.data(), result.response.size(), 0);
				}

				if (result.newHandler != nullptr)
				{
					std::lock_guard<std::mutex> lock(m_clientsMutex);
					delete m_clients[clientSocket];
					m_clients[clientSocket] = result.newHandler;
				}
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Error handling client: " << e.what() << std::endl;
	}

	closesocket(clientSocket);
	std::lock_guard<std::mutex> lock(m_clientsMutex);
	if (m_clients.find(clientSocket) != m_clients.end())
	{
		delete m_clients[clientSocket];
		m_clients.erase(clientSocket);
	}
}