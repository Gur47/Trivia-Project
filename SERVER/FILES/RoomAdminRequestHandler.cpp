#include "RoomAdminRequestHandler.h"
#include "RequestCodes.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "GameRequestHandler.h"
#include "GameManager.h"



RoomAdminRequestHandler::RoomAdminRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_user(user), m_room(room), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}


bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == GET_ROOM_STATE_REQUEST ||
        requestInfo.id == CLOSE_ROOM_REQUEST ||
        requestInfo.id == START_GAME_REQUEST;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    switch (requestInfo.id)
    {
    case CLOSE_ROOM_REQUEST: return closeRoom(requestInfo);
    case START_GAME_REQUEST: return startGame(requestInfo);
    case GET_ROOM_STATE_REQUEST: return getRoomState(requestInfo);
    default:
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid admin request" }), nullptr };
    }
}

//RequestResult RoomAdminRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo)
//{
//    GetPlayersInRoomResponse response{ m_room.getAllUsers() };
//    return { JsonResponsePacketSerializer::serializeResponse(response), std::make_unique<RoomAdminRequestHandler>(*this) };
//}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& requestInfo)
{
    m_room.closeRoom();
    m_room.removeUser(m_user);
    std::cout << "Room " << m_room.getRoomData().id
        << " now has " << m_room.getAllUsers().size()
        << " users. Active: " << m_room.isActive() << std::endl;
    CloseRoomResponse response{ 1 };

    return { JsonResponsePacketSerializer::serializeResponse(response),
             m_handlerFactory.createMenuRequestHandler(m_user) };
}


RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    try
    {
        RoomData data = m_room.getRoomData();
        std::vector<Question> questions = m_handlerFactory.getDatabase()->getQuestions(data.numOfQuestions);

        m_handlerFactory.getGameManager()->createGame(
            data.id,
            m_room.getUsers(),
            questions,
            data.timePerQuestion // ← הועבר פרמטר נוסף
        );

        m_room.startGame();

        StartGameResponse response{ 1 };
        return {
            JsonResponsePacketSerializer::serializeResponse(response),
            m_handlerFactory.createGameRequestHandler(m_user, m_room)
        };
    }
    catch (const std::exception& e)
    {
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() }), nullptr };
    }
}




RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    RoomData data = m_room.getRoomData();
    std::vector<std::string> usernames = m_room.getAllUsers();

    GetRoomStateResponse response;
    response.status = 1;
    response.players = usernames;
    response.questionCount = data.numOfQuestions;
    response.answerTimeout = data.timePerQuestion;
    response.maxPlayers = data.maxPlayers;
    response.isActive = m_room.isActive();
    response.isGameStarted = m_room.isGameStarted();

    return { JsonResponsePacketSerializer::serializeResponse(response), nullptr };
}

