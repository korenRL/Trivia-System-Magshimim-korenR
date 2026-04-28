#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "Requests.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

#define CREATE_ROOM_CODE 3
#define GET_ROOMS_CODE 4
#define JOIN_ROOM_CODE 5
#define LEAVE_ROOM_CODE 6

class MenuRequestHandler : public IRequestHandler
{
private:
	LoginManager* m_loginManager;
	RoomManager* m_roomManager;

public:
	MenuRequestHandler(LoginManager* loginManager, RoomManager* roomManager);

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;
};

