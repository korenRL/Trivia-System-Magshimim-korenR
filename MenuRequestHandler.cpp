#include "MenuRequestHandler.h"
#include <iostream>

MenuRequestHandler::MenuRequestHandler(LoginManager* loginManager) : m_loginManager(loginManager)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return true;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult result;
	result.newHandler = nullptr;

	std::cout << "MenuRequestHandler got request code: " << requestInfo.messageCode << std::endl;

	ErrorResponse err;
	err.message = "Menu not implemented yet";
	result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);

	return result;
}