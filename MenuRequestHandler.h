#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"

#include "RoomManager.h"
#include "Requests.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "StatisticsManager.h"
#include <string>

#define CREATE_ROOM_CODE 3
#define GET_ROOMS_CODE 4
#define JOIN_ROOM_CODE 5
#define LEAVE_ROOM_CODE 6
#define LOGOUT_CODE 7
#define HIGH_SCORE_CODE 8
#define PERSONAL_STATS_CODE 9

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
private:
	LoginManager* m_loginManager;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
	std::string m_username;
	RequestHandlerFactory* m_handlerFactory;

public:
	MenuRequestHandler(LoginManager* loginManager,
		RoomManager* roomManager,
		StatisticsManager* statisticsManager,
		const std::string& username,
		RequestHandlerFactory* handlerFactory);

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;
};