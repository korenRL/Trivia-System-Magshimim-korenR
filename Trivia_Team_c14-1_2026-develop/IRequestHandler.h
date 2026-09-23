#pragma once

#include <vector>
#include <string>
#include <ctime>
#include <cstddef>

class IRequestHandler;

struct RequestInfo
{
	unsigned char messageCode;
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

	virtual bool isRequestRelevant(
		const RequestInfo& requestInfo
	) const = 0;

	virtual RequestResult handleRequest(
		const RequestInfo& requestInfo
	) = 0;
};