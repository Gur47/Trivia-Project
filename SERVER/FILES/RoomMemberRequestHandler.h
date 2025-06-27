#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h" 
class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
public:
    RoomMemberRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    RequestResult leaveRoom(const RequestInfo& requestInfo);
    RequestResult getRoomState(const RequestInfo& requestInfo);
    RequestResult switchToGameHandler(const RequestInfo& requestInfo);


    LoggedUser m_user;
    Room& m_room;
    RoomManager& m_roomManager;
    RequestHandlerFactory& m_handlerFactory;
};
