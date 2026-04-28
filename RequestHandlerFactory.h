#pragma once

#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "LoginManager.h"
#include "SqliteDataBase.h"

class RequestHandlerFactory
{
public:
	static IRequestHandler* createRequestHandler(IRequestHandler* currentHandler, const RequestInfo& requestInfo, LoginManager* loginManager, SqliteDataBase* database);
};