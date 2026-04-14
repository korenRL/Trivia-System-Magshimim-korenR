#pragma once

#include <vector>
#include <string>
#include <ctime>

struct RequestInfo
{
	int messageCode;
	time_t receivalTime;
	std::vector<unsigned char> buff;
};

class IRequestHandler
{
public:
	virtual ~IRequestHandler() = default;
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
};