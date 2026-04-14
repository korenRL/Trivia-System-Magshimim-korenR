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
			m_clients[clientSocket] = new LoginRequestHandler();
		}

		std::thread t(&Communicator::handleNewClient, this, clientSocket);
		t.detach();
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{
		std::string hello = "Hello"; // "Hello" message to the client
		int sendResult = send(clientSocket, hello.c_str(), HELLO_LENGTH, 0);

		if (sendResult == SOCKET_ERROR)
		{
			std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
			throw std::runtime_error("Send failed");
		}

		char buffer[HELLO_LENGTH + 1] = { 0 };
		int received = recv(clientSocket, buffer, HELLO_LENGTH, 0);
		if (received == SOCKET_ERROR)
		{
			std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
			throw std::runtime_error("Recv failed");
		}
		else if (received == 0)
		{
			std::cout << "Client disconnected." << std::endl;
		}
		else
		{
			buffer[received] = '\0';
			std::cout << "Client sent: " << buffer << std::endl;
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