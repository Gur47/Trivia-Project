#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <memory>
#include "GameRequestHandler.h"
#include "StatisticsManager.h"
#include "StatisticsRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(LoginManager* loginManager, RoomManager* roomManager, GameManager& gameManager)
    : m_loginManager(loginManager), m_roomManager(roomManager), m_gameManager(gameManager), m_statisticsManager(loginManager->getDatabase())
{}

std::unique_ptr<LoginRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::make_unique<LoginRequestHandler>(m_loginManager, *this);
}


std::unique_ptr<MenuRequestHandler> RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return std::make_unique<MenuRequestHandler>(user, *m_roomManager, *this);
}

std::unique_ptr<RoomAdminRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, Room& room)
{
    return std::make_unique<RoomAdminRequestHandler>(user, room, *m_roomManager, *this);
}

std::unique_ptr<RoomMemberRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, Room& room)
{
    return std::make_unique<RoomMemberRequestHandler>(user, room, *m_roomManager, *this);
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return *m_roomManager;
}

std::unique_ptr<GameRequestHandler> RequestHandlerFactory::createGameRequestHandler(const LoggedUser& user, Room& room)
{
    return std::make_unique<GameRequestHandler>(user, room, m_gameManager, *this);
}


GameManager* RequestHandlerFactory::getGameManager()
{
    return &m_gameManager;
}

IDatabase* RequestHandlerFactory::getDatabase()
{
    return m_loginManager->getDatabase(); // נניח של־LoginManager יש פונקציה כזו
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return m_statisticsManager;
}

IRequestHandler* RequestHandlerFactory::createStatisticsRequestHandler(const LoggedUser& user)
{
    return new StatisticsRequestHandler(*this, user);
}
