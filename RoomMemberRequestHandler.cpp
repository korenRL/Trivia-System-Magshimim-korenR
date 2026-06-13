#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "GameRequestHandler.h"
#include "structs.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(Room room, LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_room(room), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.messageCode == RequestCode::LEAVE_ROOM_REQ ||
        requestInfo.messageCode == RequestCode::GET_ROOM_STATE_REQ;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    if (requestInfo.messageCode == RequestCode::LEAVE_ROOM_REQ)
    {
        return leaveRoom(requestInfo);
    }
    else if (requestInfo.messageCode == RequestCode::GET_ROOM_STATE_REQ)
    {
        return getRoomState(requestInfo);
    }

    return RequestResult{ std::vector<unsigned char>(), nullptr };
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& requestInfo)
{
    m_roomManager.leaveRoom(m_room.metadata.id, m_user.username);

    LeaveRoomResponse res;
    res.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeLeaveRoomResponse(res);
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.username);

    return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
	RequestResult result;
	GetRoomStateResponse res;

	if (!m_roomManager.roomExists(m_room.metadata.id))
	{
		res.status = 0;
		res.hasGameBegun = false;
		res.players = {};
		res.answerCount = 0;
		res.answerTimeout = 0;

		result.response = JsonResponsePacketSerializer::serializeGetRoomStateResponse(res);
		result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.username);
		return result;
	}

	Room& room = m_roomManager.getRoom(m_room.metadata.id);

	res.status = 1;
	res.hasGameBegun = room.metadata.isActive == 1;
	res.players = room.getAllUsers();
	res.answerCount = room.metadata.numOfQuestionsInGame;
	res.answerTimeout = room.metadata.timePerQuestion;

	result.response = JsonResponsePacketSerializer::serializeGetRoomStateResponse(res);

	if (room.metadata.isActive == 1)
	{
		result.newHandler = m_handlerFactory.createGameRequestHandler(m_user, room.metadata.id);
	}
	else
	{
		result.newHandler = this;
	}

	return result;
}