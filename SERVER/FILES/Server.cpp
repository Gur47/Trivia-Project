#include "Server.h"
#include <iostream>

Server::Server()
    : m_database(),
    m_loginManager(&m_database),
    m_roomManager(&m_database),
    m_gameManager(),
    m_handlerFactory(&m_loginManager, &m_roomManager, m_gameManager),
    m_communicator(m_handlerFactory)
{
    std::cout << "Initializing server..." << std::endl;
}


void Server::run()
{
    std::cout << "Server is running..." << std::endl;
    m_communicator.startHandleRequests();
}
