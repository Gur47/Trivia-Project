#pragma once
#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"
#include "SqliteDatabase.h"

class Server
{
public:
    Server();
    void run();

private:
    SqliteDatabase m_database;
    LoginManager m_loginManager;
    RequestHandlerFactory m_handlerFactory;
    Communicator m_communicator;
    RoomManager m_roomManager;
    GameManager m_gameManager;


};
