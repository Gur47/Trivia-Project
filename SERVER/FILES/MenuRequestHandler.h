#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RoomManager.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
    MenuRequestHandler(LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    RequestResult createRoom(const RequestInfo& requestInfo);
    RequestResult getRooms(const RequestInfo& requestInfo);
    RequestResult getPlayersInRoom(const RequestInfo& requestInfo);
    RequestResult joinRoom(const RequestInfo& requestInfo);

    LoggedUser m_user;
    RoomManager& m_roomManager;
    RequestHandlerFactory& m_handlerFactory;
};