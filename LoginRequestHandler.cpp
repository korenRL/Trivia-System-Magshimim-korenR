


#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <iostream>

LoginRequestHandler::LoginRequestHandler(LoginManager* loginManager, RequestHandlerFactory* handlerFactory)
	: m_loginManager(loginManager), m_handlerFactory(handlerFactory)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult result;

	try
	{
		if (requestInfo.messageCode == LOGIN_CODE)
		{
			result = login(requestInfo);
		}
		else if (requestInfo.messageCode == SIGNUP_CODE)
		{
			result = signup(requestInfo);
		}
		else
		{
			ErrorResponse err{ "Invalid request" };
			result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
			result.newHandler = nullptr;
		}
	}
	catch (const std::exception& e)
	{
		ErrorResponse err{ e.what() };
		result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
		result.newHandler = nullptr;
	}

	return result;
}

RequestResult LoginRequestHandler::login(const RequestInfo& requestInfo)
{
	RequestResult result;
	LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializerLoginRequest(requestInfo);

	int status = m_loginManager->login(loginRequest.username, loginRequest.password);

	LoginResponse response;
	response.status = status;

	result.response = JsonResponsePacketSerializer::serializeLoginResponse(response);

	if (status == 1)
		result.newHandler = m_handlerFactory->createMenuRequestHandler(loginRequest.username);
	else
		result.newHandler = this;

	return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& requestInfo)
{
	RequestResult result;
	SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializerSignUpRequest(requestInfo);

	int status = m_loginManager->signup(signupRequest.username, signupRequest.password, signupRequest.email);

	SignupResponse response;
	response.status = status;

	result.response = JsonResponsePacketSerializer::serializeSignupResponse(response);
	result.newHandler = this;

	return result;
}