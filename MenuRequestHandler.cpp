#include "MenuRequestHandler.h"
#include "structs.h"
#include <iostream>

MenuRequestHandler::MenuRequestHandler(LoginManager* loginManager, RoomManager* roomManager, StatisticsManager* statisticsManager) : m_loginManager(loginManager), m_roomManager(roomManager), m_statisticsManager(statisticsManager)
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
		CreateRoomRequest req = JsonRequestPacketDeserializer::deserializerCreateRoomRequest(requestInfo);
		unsigned int roomId = m_roomManager->createRoom(req.roomName, req.maxPlayers, req.questionCount, req.answerCooldown);
		
		CreateRoomResponse res;
		res.status = 1;
		res.roomId = roomId;
		result.response = JsonResponsePacketSerializer::serializeCreateRoomResponse(res);
	}
	else if (requestInfo.messageCode == GET_ROOMS_CODE)
	{
		GetRoomsResponse res;
		res.rooms = m_roomManager->getRooms();
		result.response = JsonResponsePacketSerializer::serializeGetRoomsResponse(res);
	}
	else if (requestInfo.messageCode == JOIN_ROOM_CODE)
	{
		JoinRoomRequest req = JsonRequestPacketDeserializer::deserializerJoinRoomRequest(requestInfo);
		bool success = m_roomManager->joinRoom(req.roomId, "");
		JoinRoomResponse res{ success ? 1 : 0 };
		result.response = JsonResponsePacketSerializer::serializeJoinRoomResponse(res);
	}
	else if (requestInfo.messageCode == LEAVE_ROOM_CODE)
	{
		LeaveRoomRequest req = JsonRequestPacketDeserializer::deserializerLeaveRoomRequest(requestInfo);
		m_roomManager->leaveRoom(req.roomId, "");
		LeaveRoomResponse res{ 1 };
		result.response = JsonResponsePacketSerializer::serializeLeaveRoomResponse(res);
	}

	std::cout << "MenuRequestHandler got request code: " << requestInfo.messageCode << std::endl;

	ErrorResponse err;
	err.message = "Menu not implemented yet";
	result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);

	return result;
}