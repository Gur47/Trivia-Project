#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestInfo.h"
#include "RequestResult.h"

class RequestHandlerFactory;

// login request handler
class LoginRequestHandler : public IRequestHandler
{
public:
    LoginRequestHandler(LoginManager* loginManager, RequestHandlerFactory& handlerFactory);
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;


private:
    LoginManager* m_loginManager;
    RequestHandlerFactory& m_handlerFactory;
    RequestResult signup(const RequestInfo& requestInfo);
    RequestResult login(const RequestInfo& requestInfo);
    RequestResult getUserList(const RequestInfo& requestInfo);


};
