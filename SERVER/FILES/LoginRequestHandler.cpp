#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "LoginRequest.h"
#include "SignupRequest.h"
#include "ResponseCodes.h"
#include "RequestResult.h"
#include "RequestCodes.h"
#include <iostream>
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"



LoginRequestHandler::LoginRequestHandler(LoginManager* loginManager, RequestHandlerFactory& handlerFactory)
    : m_loginManager(loginManager), m_handlerFactory(handlerFactory)
{
}


bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == LOGIN_REQUEST || 
           requestInfo.id == SIGNUP_REQUEST ||
           requestInfo.id == USERS_LIST_REQUEST;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    std::cout << "[Server] Received request. ID = " << requestInfo.id << std::endl;
    switch (requestInfo.id)
    {
    case LOGIN_REQUEST:
        return login(requestInfo);
    case SIGNUP_REQUEST:
        return signup(requestInfo);
    case USERS_LIST_REQUEST:
        return getUserList(requestInfo);
    }

    return createErrorResult("Invalid request ID");
}


RequestResult LoginRequestHandler::login(const RequestInfo& requestInfo)
{
    LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
    std::cout << "[Server] Login for user: " << req.username << std::endl;
    bool success = m_loginManager->login(req.username, req.password);

    LoginResponse response{ success ? 1 : 0 };
    auto buffer = JsonResponsePacketSerializer::serializeResponse(response);

    if (success)
    {
        LoggedUser user(req.username);
        return { buffer, m_handlerFactory.createMenuRequestHandler(user) };
    }
    return { buffer, nullptr };
}

RequestResult LoginRequestHandler::signup(const RequestInfo& requestInfo)
{
    SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
    std::cout << "[Server] Signup for user: " << req.username << std::endl;

    bool success = m_loginManager->signup(req.username, req.password, req.email);

    SignupResponse response{ success ? 1 : 0 };
    auto buffer = JsonResponsePacketSerializer::serializeResponse(response);
    return RequestResult(buffer, nullptr);
}

RequestResult LoginRequestHandler::getUserList(const RequestInfo& requestInfo)
{
    std::vector<std::string> users = m_loginManager->getAllUsers();

    UsersListResponse response{ users };
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    return { buffer, nullptr };
}


