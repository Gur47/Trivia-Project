#include "RoomMemberRequestHandler.h"
#include "RequestCodes.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include "Responses.h"
#include "GameRequestHandler.h"


RoomMemberRequestHandler::RoomMemberRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_user(user), m_room(room), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}


bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == GET_ROOM_STATE_REQUEST ||
        requestInfo.id == LEAVE_ROOM_REQUEST ||
        requestInfo.id == GET_QUESTION_REQUEST;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    switch (requestInfo.id)
    {
    case GET_ROOM_STATE_REQUEST: return getRoomState(requestInfo);
    case LEAVE_ROOM_REQUEST: return leaveRoom(requestInfo);
    case GET_QUESTION_REQUEST: return switchToGameHandler(requestInfo);
    default:
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid room member request" }), nullptr };
    }
}


RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& requestInfo)
{
    m_room.removeUser(m_user);
    std::cout << "Room " << m_room.getRoomData().id
        << " now has " << m_room.getAllUsers().size()
        << " users. Active: " << m_room.isActive() << std::endl;
    LeaveRoomResponse response{ 1 }; // 1 = success
    return {
        JsonResponsePacketSerializer::serializeResponse(response),
        m_handlerFactory.createMenuRequestHandler(m_user)
    };
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo)
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

    auto buffer = JsonResponsePacketSerializer::serializeResponse(response);

    // אם המשחק התחיל – מחליפים ל־GameRequestHandler
    if (m_room.isGameStarted())
    {
        std::cout << "[Server] Switching RoomMemberHandler -> GameRequestHandler (during polling)" << std::endl;
        auto nextHandler = m_handlerFactory.createGameRequestHandler(m_user, m_room);
        return { buffer, std::move(nextHandler) };
    }

    // אחרת, ממשיכים עם ה־handler הנוכחי
    return { buffer, nullptr };
}




RequestResult RoomMemberRequestHandler::switchToGameHandler(const RequestInfo& requestInfo)
{
    if (!m_room.isGameStarted())
    {
        // המשחק עדיין לא התחיל – לא מחליף handler
        return {
            JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Game has not started yet." }),
            nullptr
        };
    }

    StartGameResponse response{ 1 }; // או קוד אחר שתואם ל־GameWindow
    auto buffer = JsonResponsePacketSerializer::serializeResponse(response);
    auto nextHandler = m_handlerFactory.createGameRequestHandler(m_user, m_room);
    return { buffer, std::move(nextHandler) };
}


