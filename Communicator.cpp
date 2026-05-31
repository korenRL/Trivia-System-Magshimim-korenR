
//#include "Communicator.h"
//#include <iostream>
//#include <string>
//<<<<<<< HEAD
//
//Communicator::Communicator(RequestHandlerFactory* handlerFactory) : m_handlerFactory(handlerFactory)
//=======
//#include <vector>
//
//Communicator::Communicator(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)
//>>>>>>> origin/develop
//{
//	WSADATA wsaData;
//	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (result != 0)
//	{
//		throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
//	}
//
//	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (m_serverSocket == INVALID_SOCKET)
//	{
//		throw std::runtime_error("Error creating socket: " + std::to_string(WSAGetLastError()));
//	}
//}
//
//Communicator::~Communicator()
//{
//	if (m_serverSocket != INVALID_SOCKET)
//	{
//		closesocket(m_serverSocket);
//	}
//
//	for (auto& pair : m_clients)
//	{
//		closesocket(pair.first);
//		delete pair.second;
//	}
//
//	m_clients.clear();
//	WSACleanup();
//}
//
//void Communicator::bindAndListen()
//{
//	struct sockaddr_in serverAddr;
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_port = htons(SERVER_PORT);
//	serverAddr.sin_addr.s_addr = INADDR_ANY;
//
//	int bindResult = bind(m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
//	if (bindResult == SOCKET_ERROR)
//	{
//		throw std::runtime_error("Bind failed with error: " + std::to_string(WSAGetLastError()));
//	}
//
//	int listenResult = listen(m_serverSocket, SOMAXCONN);
//	if (listenResult == SOCKET_ERROR)
//	{
//		throw std::runtime_error("Listen failed with error: " + std::to_string(WSAGetLastError()));
//	}
//
//	std::cout << "Listening on port " << SERVER_PORT << "..." << std::endl;
//}
//
//void Communicator::startHandleRequests()
//{
//	bindAndListen();
//
//	while (true)
//	{
//		struct sockaddr_in clientAddr;
//		int clientAddrSize = sizeof(clientAddr);
//
//		SOCKET clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
//		if (clientSocket == INVALID_SOCKET)
//		{
//			std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
//			continue;
//		}
//
//		{
//			std::lock_guard<std::mutex> lock(m_clientsMutex);
//<<<<<<< HEAD
//			m_clients[clientSocket] = m_handlerFactory->createLoginRequestHandler();
//=======
//			m_clients[clientSocket] = m_handlerFactory.createLoginRequestHandler();
//>>>>>>> origin/develop
//		}
//
//		std::thread t(&Communicator::handleNewClient, this, clientSocket);
//		t.detach();
//	}
//}

//void Communicator::handleNewClient(SOCKET clientSocket)
//{
//	try
//	{
//<<<<<<< HEAD
//		
//		std::cout << "Client connected and handshake done." << std::endl;
//
//=======
//>>>>>>> origin/develop
//		while (true)
//		{
//			//getting the first code, this is a char sended before len of payload
//			//after getting that you need to get len of payload and then by the len of the payload
//			//you are getting the payload you need
//			unsigned char header[5] = { 0 };
//<<<<<<< HEAD
//			int bytesRecevied = recv(clientSocket, (char*)&header, 5, 0);
//=======
//>>>>>>> origin/develop
//
//			int bytesReceived = 0;
//			while (bytesReceived < 5)
//			{
//				int res = recv(clientSocket, (char*)header + bytesReceived, 5 - bytesReceived, 0);
//				if (res <= 0) break;
//				bytesReceived += res;
//			}
//
//			if (bytesReceived < 5)
////			{
//				break;
//			}
//			std::cout << header << std::endl;
//			std::cout << header[0] << std::endl;
//			RequestInfo requestInfo;
//			requestInfo.messageCode = header[0];
//
//			unsigned int dataSize = 0;
//			dataSize |= (unsigned int)header[1] << 24;
//			dataSize |= (unsigned int)header[2] << 16;
//			dataSize |= (unsigned int)header[3] << 8;
//			dataSize |= (unsigned int)header[4];
//
//			std::vector<unsigned char> bufferVec(dataSize);
//			if (dataSize > 0)
//			{
//				unsigned int payloadReceived = 0;
//				while (payloadReceived < dataSize)
//				{
//					int res = recv(clientSocket, (char*)bufferVec.data() + payloadReceived, dataSize - payloadReceived, 0);
//					if (res <= 0) break;
//					payloadReceived += res;
//				}
//
//				if (payloadReceived < dataSize)
//				{
//					break;
//				}
//			}
//
//			requestInfo.buff = bufferVec;
//			requestInfo.receivalTime = std::time(NULL);
//
//			IRequestHandler* handler = nullptr;
//			{
//				std::lock_guard<std::mutex> lock(m_clientsMutex);
//				if (m_clients.find(clientSocket) != m_clients.end())
//				{
//					handler = m_clients[clientSocket];
//				}
//			}
//
//			if (handler && handler->isRequestRelevant(requestInfo))
//			{
//				RequestResult result = handler->handleRequest(requestInfo);
//
//				if (!result.response.empty())
//				{
//					send(clientSocket, (char*)result.response.data(), result.response.size(), 0);
//				}
//
//				if (result.newHandler != nullptr && result.newHandler != handler)
//				{
//					std::lock_guard<std::mutex> lock(m_clientsMutex);
//					delete m_clients[clientSocket]; 
//					m_clients[clientSocket] = result.newHandler;
//				}
//			}
//		}
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Error handling client: " << e.what() << std::endl;
//	}
//
//	closesocket(clientSocket);
//	std::lock_guard<std::mutex> lock(m_clientsMutex);
//	if (m_clients.find(clientSocket) != m_clients.end())
//	{
//		delete m_clients[clientSocket];
//		m_clients.erase(clientSocket);
//	}
//}


#include "Communicator.h"
#include <iostream>
#include <string>
#include <vector>

Communicator::Communicator(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)
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

		{
			std::lock_guard<std::mutex> lock(m_clientsMutex);
			m_clients[clientSocket] = m_handlerFactory.createLoginRequestHandler();
		}

		std::thread t(&Communicator::handleNewClient, this, clientSocket);
		t.detach();
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{
		std::cout << "Client connected and handshake done." << std::endl;

		while (true)
		{
			//getting the first code, this is a char sended before len of payload
			//after getting that you need to get len of payload and then by the len of the payload
			//you are getting the payload you need
			unsigned char header[5] = { 0 };

			int bytesReceived = 0;
			while (bytesReceived < 5)
			{
				int res = recv(clientSocket, (char*)header + bytesReceived, 5 - bytesReceived, 0);
				if (res <= 0) break;
				bytesReceived += res;
			}

			if (bytesReceived < 5)
			{
				break;
			}
			std::cout << header << std::endl;
			std::cout << header[0] << std::endl;

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
				unsigned int payloadReceived = 0;
				while (payloadReceived < dataSize)
				{
					int res = recv(clientSocket, (char*)bufferVec.data() + payloadReceived, dataSize - payloadReceived, 0);
					if (res <= 0) break;
					payloadReceived += res;
				}

				if (payloadReceived < dataSize)
				{
					break;
				}
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

				if (result.newHandler != nullptr && result.newHandler != handler)
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