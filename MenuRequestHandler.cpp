#include "MenuRequestHandler.h"
#include <iostream>

MenuRequestHandler::MenuRequestHandler(LoginManager* loginManager, RoomManager* roomManager) : m_loginManager(loginManager), m_roomManager(roomManager)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	if (requestInfo.messageCode == CREATE_ROOM_CODE || requestInfo.messageCode == GET_ROOMS_CODE || requestInfo.messageCode == JOIN_ROOM_CODE || requestInfo.messageCode == LEAVE_ROOM_CODE)
	{
		return true;
	}
	return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult result;
	result.newHandler = nullptr;

	if (requestInfo.messageCode == CREATE_ROOM_CODE)
	{
		
	}
	else if (requestInfo.messageCode == GET_ROOMS_CODE)
	{

	}

	std::cout << "MenuRequestHandler got request code: " << requestInfo.messageCode << std::endl;

	ErrorResponse err;
	err.message = "Menu not implemented yet";
	result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);

	return result;
}