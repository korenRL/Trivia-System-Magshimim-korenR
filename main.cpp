#include "Server.h"
#include <iostream>

int main()
{
	try
	{
		Server myServer;
		myServer.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception in main: " << e.what() << std::endl;
	}

	return 0;
}