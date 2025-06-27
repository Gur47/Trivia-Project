#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"

class StatisticsRequestHandler : public IRequestHandler
{
public:
    StatisticsRequestHandler(RequestHandlerFactory& factory, const LoggedUser& user);

    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    RequestResult getPersonalStats();
    RequestResult getGeneralStats();

    RequestHandlerFactory& m_handlerFactory;
    LoggedUser m_user;
};
