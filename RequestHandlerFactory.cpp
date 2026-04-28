#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"

IRequestHandler* RequestHandlerFactory::createRequestHandler(IRequestHandler* currentHandler, const RequestInfo& requestInfo, LoginManager* loginManager, SqliteDataBase* database)
{
	if (dynamic_cast<LoginRequestHandler*>(currentHandler) != nullptr)
	{
		return new MenuRequestHandler(loginManager, nullptr);
	}

	return nullptr;
}