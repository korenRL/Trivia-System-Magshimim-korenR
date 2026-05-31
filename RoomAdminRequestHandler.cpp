#include "RoomAdminRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "structs.h"
RoomAdminRequestHandler::RoomAdminRequestHandler(Room room, LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_room(room), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.messageCode == RequestCode::CLOSE_ROOM_REQ ||
        requestInfo.messageCode == RequestCode::START_GAME_REQ ||
        requestInfo.messageCode == RequestCode::GET_ROOM_STATE_REQ;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    if (requestInfo.messageCode == RequestCode::CLOSE_ROOM_REQ)
    {
        return closeRoom(requestInfo);
    }
    else if (requestInfo.messageCode == RequestCode::START_GAME_REQ)
    {
        return startGame(requestInfo);
    }
    else if (requestInfo.messageCode == RequestCode::GET_ROOM_STATE_REQ)
    {
        return getRoomState(requestInfo);
    }

    return RequestResult{ std::vector<unsigned char>(), nullptr };
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& requestInfo)
{
    CloseRoomResponse res;
    res.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeCloseRoomResponse(res);
    result.newHandler = nullptr; 

    return result;
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    StartGameResponse res;
    res.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeStartGameResponse(res);
    result.newHandler = this;

    return result;
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& requestInfo)
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