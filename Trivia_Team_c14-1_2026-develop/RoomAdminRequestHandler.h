#pragma once
#include "IRequestHandler.h"
#include "RoomManager.h"
#include "LoginManager.h"

class RequestHandlerFactory; 

class RoomAdminRequestHandler : public IRequestHandler
{
public:
    RoomAdminRequestHandler(
        Room room, 
        LoggedUser user, 
        RoomManager& roomManager, 
        RequestHandlerFactory& handlerFactory
    );

    bool isRequestRelevant(
        const RequestInfo& requestInfo
    ) const override;

    RequestResult handleRequest(
        const RequestInfo& requestInfo
    ) override;

private:
    Room m_room;
    LoggedUser m_user;
    RoomManager& m_roomManager;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult closeRoom(const RequestInfo& requestInfo);
    RequestResult startGame(const RequestInfo& requestInfo);
    RequestResult getRoomState(const RequestInfo& requestInfo);
};