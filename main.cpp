#include <iostream>

#include "HttpServer.h"

int main(int argc, char* argv[])
{
	Logger* logger = new Logger(std::cout);

	std::string address = "0.0.0.0";
	std::string port = "1080";
	std::string dir;
	
	//expected input params: -h <ip> -p <port> -d <directory>
	if (argc < 7 ||
		strcmp(argv[1], "-h") ||
		strcmp(argv[3], "-p") || 
		strcmp(argv[5], "-d"))
	{
		logger->Warn("Command line args are empty or have wrong format."
			"Using defaut addresses!");
	}
	else
	{
		address = argv[2];
		port = argv[4];
		dir = argv[6];
	}

	Server s(dir, address, port, logger);
	s.Start();
	s.Stop();
}



