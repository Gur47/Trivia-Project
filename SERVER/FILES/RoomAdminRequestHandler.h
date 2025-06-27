#pragma once

#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
public:
    RoomAdminRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    RequestResult closeRoom(const RequestInfo& requestInfo);
    RequestResult startGame(const RequestInfo& requestInfo); 
    RequestResult getRoomState(const RequestInfo& requestInfo);

    RoomManager& m_roomManager;
    LoggedUser m_user;
    Room& m_room;
    RequestHandlerFactory& m_handlerFactory;
};
