#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <iostream>

LoginRequestHandler::LoginRequestHandler(LoginManager* loginManager) : m_loginManager(loginManager)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	if (requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE)
	{
		return true;
	}
	return false;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult result;
	result.newHandler = nullptr;

	if (requestInfo.messageCode == LOGIN_CODE)
	{
		LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializerLoginRequest(requestInfo);

		int status = m_loginManager->login(loginRequest.username, loginRequest.password);
		LoginResponse response;
		response.status = status;

		result.response = JsonResponsePacketSerializer::serializeLoginResponse(response);
	}
	else if (requestInfo.messageCode == SIGNUP_CODE)
	{
		SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializerSignUpRequest(requestInfo);
		int status = m_loginManager->signup(signupRequest.username, signupRequest.password, signupRequest.email);

		SignupResponse response;
		response.status = status;

		result.response = JsonResponsePacketSerializer::serializeSignupResponse(response);
	}
	else
	{
		ErrorResponse err;
		err.message = "Invalid request";
		result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
	}

	result.newHandler = RequestHandlerFactory::createRequestHandler(this, requestInfo, m_loginManager, nullptr);
	return result;
}