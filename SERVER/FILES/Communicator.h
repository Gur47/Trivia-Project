#pragma once
#include <iostream>
#include <winsock2.h>
#include <map>
#include <memory>
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

#include <unordered_map>
#include <mutex>
#include <string>


#define PORT 20235

// handles client connections and request routing
class Communicator
{
public:
    Communicator(RequestHandlerFactory& handlerFactory);
    ~Communicator();
    void startHandleRequests();
    void bindAndListen();
    void handleNewClient(SOCKET client_socket);

private:
    SOCKET m_serverSocket;
    std::map<SOCKET, std::unique_ptr<IRequestHandler>> m_clients;
    RequestHandlerFactory& m_handlerFactory;

};
