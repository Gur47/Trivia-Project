#include "Communicator.h"
#include <iostream>
#include <string>
#include <thread>
#include "IDatabase.h"
#include "RequestInfo.h"
#include "LoginRequest.h"
#include "ResponseCodes.h"
#include "Buffer.h"
#include <chrono>

#define PORT 20235

Communicator::Communicator(RequestHandlerFactory& handlerFactory)
    : m_handlerFactory(handlerFactory)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
        exit(1);
    }
}

Communicator::~Communicator()
{
    WSACleanup();
}



void Communicator::startHandleRequests()
{
    try {
        bindAndListen();
        while (true)
        {
            SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET)
            {
                std::cerr << "Failed to accept client." << std::endl;
                continue;
            }

            std::thread clientThread(&Communicator::handleNewClient, this, clientSocket);
            clientThread.detach();
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
}

void Communicator::bindAndListen()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(PORT); // agreed port

    if (bind(m_serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed." << std::endl;
        closesocket(m_serverSocket);
        return;
    }

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed." << std::endl;
        closesocket(m_serverSocket);
        return;
    }

    std::cout << "Server is listening on port 20235..." << std::endl;
}

void Communicator::handleNewClient(const SOCKET clientSocket)
{
    std::cout << "[Server] New client connected. SOCKET: " << clientSocket << std::endl;

    std::unique_ptr<IRequestHandler> handler = m_handlerFactory.createLoginRequestHandler();

    while (true)
    {
        try
        {
            std::cout << "[Server] Awaiting requestId..." << std::endl;
            unsigned char requestId;
            int readBytes = recv(clientSocket, reinterpret_cast<char*>(&requestId), 1, 0);
            if (readBytes <= 0)
            {
                std::cerr << "[Server] Failed to read requestId. Ending client session." << std::endl;
                break;
            }
            std::cout << "[Server] Received requestId = " << (int)requestId << std::endl;

            std::cout << "[Server] Awaiting size buffer..." << std::endl;
            unsigned char sizeBuffer[4];
            recv(clientSocket, reinterpret_cast<char*>(sizeBuffer), 4, 0);
            int messageSize = (sizeBuffer[0] << 24) | (sizeBuffer[1] << 16) | (sizeBuffer[2] << 8) | sizeBuffer[3];
            std::cout << "[Server] messageSize = " << messageSize << std::endl;

            std::cout << "[Server] Awaiting JSON data payload..." << std::endl;
            std::vector<unsigned char> data;
            if (messageSize > 0)
            {
                data.resize(messageSize);
                recv(clientSocket, reinterpret_cast<char*>(data.data()), messageSize, 0);
            }

            std::cout << "[Server] Received full payload." << std::endl;

            RequestInfo requestInfo;
            requestInfo.id = requestId;
            requestInfo.buffer = data;
            requestInfo.receivedTime = time(nullptr);

            std::cout << "[Server] Dispatching request to handler..." << std::endl;
            if (handler->isRequestRelevant(requestInfo))
            {
                std::cout << "[Server] Request is relevant. Processing..." << std::endl;
                RequestResult result = handler->handleRequest(requestInfo);

                std::cout << "[Server] Sending response of size: " << result.response.getData().size() << std::endl;
                std::cout << "[Server] Sending response of data: " << result.response.getData().data() << std::endl;

                send(clientSocket, reinterpret_cast<const char*>(result.response.getData().data()), result.response.getData().size(), 0);

                if (result.newHandler)
                {
                    std::cout << "[Server] Handler switched to new handler." << std::endl;
                    handler = std::move(result.newHandler);
                }
                else
                {
                    std::cout << "[Server] Continuing with current handler." << std::endl;
                }
            }
            else
            {
                std::cerr << "[Server] Request is NOT relevant. Closing connection." << std::endl;
                break;
            }
        }
        catch (...)
        {
            std::cerr << "[Server] Error while handling client " << clientSocket << std::endl;
            break;
        }
    }

    std::cout << "[Server] Closing socket for client " << clientSocket << std::endl;
    closesocket(clientSocket);
}

