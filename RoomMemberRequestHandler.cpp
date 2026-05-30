#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
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
    LeaveRoomResponse res;
    res.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeLeaveRoomResponse(res);
    result.newHandler = nullptr; 

    return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    GetRoomStateResponse res;
    res.status = 1;
    res.hasGameBegun = false;
    res.players = {};
    res.answerCount = 0;
    res.answerTimeout = 10;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeGetRoomStateResponse(res);
    result.newHandler = this;

    return result;
}