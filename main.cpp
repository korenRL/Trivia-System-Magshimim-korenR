#include "Server.h"
#include <iostream>

int main()
{
	try
	{
		Server server;
		server.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}