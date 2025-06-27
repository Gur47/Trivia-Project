#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class GameRequestHandler : public IRequestHandler
{
public:
    GameRequestHandler(const LoggedUser& user, Room& room, GameManager& m_gameManager, RequestHandlerFactory& factory);
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
    RequestResult getQuestion(const RequestInfo& requestInfo);
    RequestResult submitAnswer(const RequestInfo& requestInfo);
    std::unordered_map<std::string, time_t> m_lastQuestionTime;
    RequestResult getGameResults(const RequestInfo& requestInfo); 



private:
    LoggedUser m_user;
    Room& m_room;
    GameManager& m_gameManager;
    RequestHandlerFactory& m_handlerFactory;

};
