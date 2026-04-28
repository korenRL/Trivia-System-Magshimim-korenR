#include "RequestHandlerFactory.h"

IRequestHandler* RequestHandlerFactory::createRequestHandler(IRequestHandler* currentHandler, const RequestInfo& requestInfo, LoginManager* loginManager, SqliteDataBase* database)
{
	if (dynamic_cast<LoginRequestHandler*>(currentHandler) != nullptr)
	{
		if (requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE)
		{
			return nullptr;
		}
	}

	return nullptr;
}