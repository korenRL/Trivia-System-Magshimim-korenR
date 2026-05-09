#include "Communicator.h"
#include <iostream>
#include <string>
#include "StatisticsManager.h"
#include "JsonResponsePacketSerializer.h"

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
	m_roomManager = new RoomManager();
	m_statisticsManager = new StatisticsManager(m_database);
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
	delete m_roomManager;
	delete m_statisticsManager;

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

bool Communicator::receiveExact(SOCKET socket, char* buffer, int size)
{
	int totalReceived = 0;

	while (totalReceived < size)
	{
		int currentReceived = recv(socket, buffer + totalReceived, size - totalReceived, 0);
		if (currentReceived <= 0)
		{
			return false;
		}

		totalReceived += currentReceived;
	}

	return true;
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	try 
	{
		while (true)
		{
			RequestInfo requestInfo;
			char codeChar = 0;

			if (!receiveExact(clientSocket, &codeChar, 1))
			{
				std::cout << "Client Disconnected." << std::endl;
				break;
			}

			unsigned char messageCode = codeChar - '0';
			requestInfo.messageCode = messageCode;

			std::string lengthStr;
			char currentChar = 0;

			bool failedReading = false;
			while (true)
			{
				if (!receiveExact(clientSocket, &currentChar, 1))
				{
					std::cout << "Client disconnected while reading length." << std::endl;
					failedReading = true;
					break;
				}

				if (currentChar == '{')
				{
					break;
				}

				lengthStr += currentChar;
			}
			
			if (failedReading)
			{
				break;
			}

			unsigned int dataSize = std::stoi(lengthStr);

			std::vector<unsigned char> bufferVec(dataSize);
			bufferVec[0] = '{';

			if (dataSize > 1 && !receiveExact(clientSocket, (char*)bufferVec.data() + 1, dataSize - 1))
			{
				std::cout << "Client disconnected while sending data." << std::endl;
				break;
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
			else
			{
				ErrorResponse err;
				err.message = "Invalid request";
				std::vector<unsigned char> response = JsonResponsePacketSerializer::serializeErrorResponse(err);
				send(clientSocket, (char*)response.data(), response.size(), 0);
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