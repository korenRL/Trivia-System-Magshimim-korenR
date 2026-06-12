#include "RoomAdminRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "GameRequestHandler.h"
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
    m_roomManager.deleteRoom(m_room.metadata.id);

    CloseRoomResponse res;
    res.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeCloseRoomResponse(res);
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.username);

    return result;
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    Room& room = m_roomManager.getRoom(m_room.metadata.id);
    m_handlerFactory.getGameManager()->createGame(room);
    room.metadata.isActive = 1;

    StartGameResponse res;
    res.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeStartGameResponse(res);
    result.newHandler = m_handlerFactory.createGameRequestHandler(m_user);

    return result;
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    Room& room = m_roomManager.getRoom(m_room.metadata.id);

    GetRoomStateResponse res;
    res.status = 1;
    res.hasGameBegun = room.metadata.isActive == 1;
    res.players = room.getAllUsers();
    res.answerCount = room.metadata.numOfQuestionsInGame;
    res.answerTimeout = room.metadata.timePerQuestion;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeGetRoomStateResponse(res);
    result.newHandler = this;

    return result;
}