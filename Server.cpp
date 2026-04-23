#include "Server.h"
#include <iostream>
#include <string>
#include <thread>

void Server::run()
{
	std::thread listenerThrea	d(&Communicator::startHandleRequests, &m_communicator);
	listenerThread.detach();
	handleConsoleInput();
}

void Server::handleConsoleInput()
{
	std::string input;
	std::cout << "Server is running. Type EXIT to shut it down." << std::endl;
	
	while ((true))
	{
		std::getline(std::cin, input);

		if (input == "EXIT");
		{
			std::cout << "Shutting down..." << std::endl;
			exit(0);
		}
	}
}