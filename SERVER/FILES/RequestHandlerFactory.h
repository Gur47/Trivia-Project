#pragma once
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "RoomManager.h"
#include "GameManager.h"
#include "StatisticsManager.h"
#include "StatisticsRequestHandler.h"

class GameRequestHandler;

class RequestHandlerFactory
{
public:
    RequestHandlerFactory(LoginManager* loginManager, RoomManager* roomManager, GameManager& gameManager);

    std::unique_ptr<LoginRequestHandler> createLoginRequestHandler();
    std::unique_ptr<MenuRequestHandler> createMenuRequestHandler(const LoggedUser& user);
    std::unique_ptr<RoomAdminRequestHandler> createRoomAdminRequestHandler(const LoggedUser& user, Room& room);
    std::unique_ptr<RoomMemberRequestHandler> createRoomMemberRequestHandler(const LoggedUser& user, Room& room);
    std::unique_ptr<GameRequestHandler> createGameRequestHandler(const LoggedUser& user, Room& room); 

    RoomManager& getRoomManager();
    GameManager* getGameManager();
    IDatabase* getDatabase();

    StatisticsManager& getStatisticsManager();
    IRequestHandler* createStatisticsRequestHandler(const LoggedUser& user);



private:
    LoginManager* m_loginManager;
    RoomManager* m_roomManager;
    GameManager& m_gameManager;
    StatisticsManager m_statisticsManager;



};
