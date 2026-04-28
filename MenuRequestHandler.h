#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"

class MenuRequestHandler : public IRequestHandler
{
private:
	LoginManager* m_loginManager;

public:
	MenuRequestHandler(LoginManager* loginManager);

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;
};