#include "RequestHandlerFactory.h"

// בנאי המקבל רק את מסד הנתונים
RequestHandlerFactory::RequestHandlerFactory(SqliteDataBase* database)
	: m_database(database), 
	  m_loginManager(new LoginManager(database)),
	  m_statisticsManager(new StatisticsManager(database))
{
}

// הורס שמשחרר את הזיכרון (זה מה שהקומפיילר חיפש!)
RequestHandlerFactory::~RequestHandlerFactory()
{
	if (m_loginManager != nullptr)
	{
		delete m_loginManager;
		m_loginManager = nullptr;
	}
	
	if (m_statisticsManager != nullptr)
	{
		delete m_statisticsManager;
		m_statisticsManager = nullptr;
	}
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(m_loginManager);
}