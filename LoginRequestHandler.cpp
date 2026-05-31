
//#include "LoginRequestHandler.h"
//#include "RequestHandlerFactory.h"
//#include <iostream>
//
//LoginRequestHandler::LoginRequestHandler(LoginManager* loginManager, RequestHandlerFactory* handlerFactory)
//	: m_loginManager(loginManager), m_handlerFactory(handlerFactory)
//{
//}
//
//bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
//{
//	return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
//}
//
//RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
//{
//	RequestResult result;
//
//	try
//	{
//<<<<<<< HEAD
//		if (requestInfo.messageCode == LOGIN_CODE)
//		{
//			result = login(requestInfo);
//		}
//		else if (requestInfo.messageCode == SIGNUP_CODE)
//		{
//			result = signup(requestInfo);
//		}
//		else
//		{
//			ErrorResponse err{ "Invalid request" };
//			result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
//			result.newHandler = nullptr;
//		}
//=======
	/*	LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializerLoginRequest(requestInfo);
		int status = m_loginManager->login(loginRequest.username, loginRequest.password);

		LoginResponse response;
		response.status = (status == 1) ? 0 : 1;
		result.response = JsonResponsePacketSerializer::serializeLoginResponse(response);
>>>>>>> origin/develop
	}
	catch (const std::exception& e)
	{
<<<<<<< HEAD
		ErrorResponse err{ e.what() };
=======
		SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializerSignUpRequest(requestInfo);
		int status = m_loginManager->signup(signupRequest.username, signupRequest.password, signupRequest.email);

		SignupResponse response;
		response.status = (status == 1) ? 0 : 1;
		result.response = JsonResponsePacketSerializer::serializeSignupResponse(response);
	}
	else
	{
		ErrorResponse err;
		err.message = "Invalid request";
>>>>>>> origin/develop
		result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
		result.newHandler = nullptr;
	}

<<<<<<< HEAD
	return result;
}

RequestResult LoginRequestHandler::login(const RequestInfo& requestInfo)
{
	RequestResult result;
	LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializerLoginRequest(requestInfo);

	int status = m_loginManager->login(loginRequest.username, loginRequest.password);
	LoginResponse response{ (unsigned int)status };

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
	SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializerSignUpRequest(requestInfo);*/

	/*int status = m_loginManager->signup(signupRequest.username, signupRequest.password, signupRequest.email);
	SignupResponse response{ (unsigned int)status };

	result.response = JsonResponsePacketSerializer::serializeSignupResponse(response);
	result.newHandler = this;

=======
>>>>>>> origin/develop
	return result;
}*/



#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <iostream>

LoginRequestHandler::LoginRequestHandler(LoginManager* loginManager, RequestHandlerFactory* handlerFactory)
	: m_loginManager(loginManager), m_handlerFactory(handlerFactory)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	// Updated to use 'id' instead of 'messageCode'
	return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult result;

	try
	{
		// Updated to use 'id'
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
	// Adopting develop branch's status convention: 0 for success, 1 for fail
	response.status = (status == 1) ? 0 : 1;

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
	response.status = (status == 1) ? 0 : 1;

	result.response = JsonResponsePacketSerializer::serializeSignupResponse(response);
	result.newHandler = this;

	return result;
}