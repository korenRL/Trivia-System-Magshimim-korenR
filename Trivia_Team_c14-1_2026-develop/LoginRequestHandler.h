#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "Requests.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

#define LOGIN_CODE 1
#define SIGNUP_CODE 2

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
private:
	LoginManager* m_loginManager;
	RequestHandlerFactory* m_handlerFactory;

	
	RequestResult login(const RequestInfo& requestInfo);
	RequestResult signup(const RequestInfo& requestInfo);

public:
	LoginRequestHandler(LoginManager* loginManager, RequestHandlerFactory* handlerFactory);

	virtual bool isRequestRelevant(
		const RequestInfo& requestInfo
	) const override;

	virtual RequestResult handleRequest(
		const RequestInfo& requestInfo
	) override;
};