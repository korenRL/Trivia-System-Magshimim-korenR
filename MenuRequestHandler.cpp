#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "structs.h"
#include <iostream>

MenuRequestHandler::MenuRequestHandler(LoginManager* loginManager,
    RoomManager* roomManager,
    StatisticsManager* statisticsManager,
    const std::string& username,
    RequestHandlerFactory* handlerFactory)
    : m_loginManager(loginManager),
    m_roomManager(roomManager),
    m_statisticsManager(statisticsManager),
    m_username(username),
    m_handlerFactory(handlerFactory)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.messageCode == CREATE_ROOM_CODE ||
        requestInfo.messageCode == GET_ROOMS_CODE ||
        requestInfo.messageCode == JOIN_ROOM_CODE ||
        requestInfo.messageCode == LEAVE_ROOM_CODE ||
        requestInfo.messageCode == HIGH_SCORE_CODE ||
        requestInfo.messageCode == PERSONAL_STATS_CODE;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    RequestResult result;
    result.newHandler = nullptr;

    if (requestInfo.messageCode == CREATE_ROOM_CODE)
    {
        CreateRoomRequest req = JsonRequestPacketDeserializer::deserializerCreateRoomRequest(requestInfo);

        unsigned int roomId = m_roomManager->createRoom(
            req.roomName,
            req.maxPlayers,
            req.questionCount,
            req.answerCooldown);

        CreateRoomResponse res;
        res.status = 1;
        res.roomId = roomId;

        result.response = JsonResponsePacketSerializer::serializeCreateRoomResponse(res);

        m_roomManager->joinRoom(roomId, m_username);
        Room& room = m_roomManager->getRoom(roomId);
        result.newHandler = m_handlerFactory->createRoomAdminRequestHandler(LoggedUser(m_username), room);
    }
    else if (requestInfo.messageCode == GET_ROOMS_CODE)
    {
        GetRoomsResponse res;
        auto rooms = m_roomManager->getRooms();
        for (auto& r : rooms)
        {
            res.rooms.push_back(r.metadata);
        }
        result.response = JsonResponsePacketSerializer::serializeGetRoomsResponse(res);
    }
    else if (requestInfo.messageCode == JOIN_ROOM_CODE)
    {
        JoinRoomRequest req = JsonRequestPacketDeserializer::deserializerJoinRoomRequest(requestInfo);

        bool success = m_roomManager->joinRoom(req.roomId, m_username);

        JoinRoomResponse res;
        res.status = success ? 1 : 0;

        result.response = JsonResponsePacketSerializer::serializeJoinRoomResponse(res);

        if (success)
        {
            Room& room = m_roomManager->getRoom(req.roomId);
            result.newHandler = m_handlerFactory->createRoomMemberRequestHandler(LoggedUser(m_username), room);
        }
    }
    else if (requestInfo.messageCode == LEAVE_ROOM_CODE)
    {
        LeaveRoomRequest req = JsonRequestPacketDeserializer::deserializerLeaveRoomRequest(requestInfo);

        m_roomManager->leaveRoom(req.roomId, m_username);

        LeaveRoomResponse res;
        res.status = 1;

        result.response = JsonResponsePacketSerializer::serializeLeaveRoomResponse(res);
    }
    else if (requestInfo.messageCode == HIGH_SCORE_CODE)
    {
        StatisticsResponse res;
        res.statistics = m_statisticsManager->getHighScores();

        result.response = JsonResponsePacketSerializer::serializeHighScoreResponse(res);
    }
    else if (requestInfo.messageCode == PERSONAL_STATS_CODE)
    {
        StatisticsResponse res;
        res.statistics = m_statisticsManager->getPersonalStats(m_username);

        result.response = JsonResponsePacketSerializer::serializePersonalStatsResponse(res);
    }

    std::cout << "MenuRequestHandler got request code: " << (int)requestInfo.messageCode << std::endl;

    return result;
}