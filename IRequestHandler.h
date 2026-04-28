#pragma once

#include <vector>
#include <string>
#include <ctime>

class IRequestHandler;

struct RequestInfo
{
	int messageCode;
	time_t receivalTime;
	std::vector<unsigned char> buff;
};

struct RequestResult
{
	std::vector<unsigned char> response;
	IRequestHandler* newHandler;
};

class IRequestHandler
{
public:
	virtual ~IRequestHandler() = default;
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
};