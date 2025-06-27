#include "MenuRequestHandler.h"
#include "RequestCodes.h"
#include "ResponseCodes.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestResult.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"


MenuRequestHandler::MenuRequestHandler(LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    switch (requestInfo.id)
    {
    case USERS_LIST_REQUEST:  
    case JOIN_ROOM_REQUEST:  
    case CREATE_ROOM_REQUEST:  
    case GET_ROOMS_REQUEST:  
    case GET_PERSONAL_STATS_REQUEST:
    case GET_GENERAL_STATS_REQUEST:
        return true;
    default:
        return false;
    }
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    switch (requestInfo.id)
    {
    case USERS_LIST_REQUEST: return getPlayersInRoom(requestInfo);
    case JOIN_ROOM_REQUEST: return joinRoom(requestInfo);
    case CREATE_ROOM_REQUEST: return createRoom(requestInfo);
    case GET_ROOMS_REQUEST: return getRooms(requestInfo);
    case GET_PERSONAL_STATS_REQUEST:
    case GET_GENERAL_STATS_REQUEST:
        return m_handlerFactory.createStatisticsRequestHandler(m_user)->handleRequest(requestInfo);
    default:
    {
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{"Invalid menu request"}), nullptr }; //serializeErrorResponse("Invalid menu request"), nullptr };
    }
    }
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& requestInfo)
{
    CreateRoomRequest req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.buffer);

    try
    {
        std::vector<Question> availableQuestions = m_roomManager.getDatabase()->getQuestions(1000);
        if (req.questionCount > availableQuestions.size())
        {
            ErrorResponse error{ "Not enough questions in the database. Please choose a smaller number." };
            return {
                JsonResponsePacketSerializer::serializeResponse(error),
                nullptr
            };
        }
    }
    catch (const std::exception& e)
    {
        ErrorResponse error{ "Failed to check question availability." };
        return {
            JsonResponsePacketSerializer::serializeResponse(error),
            nullptr
        };
    }


    RoomData data;
    data.name = req.roomName;
    data.maxPlayers = req.maxUsers;
    data.timePerQuestion = req.answerTimeout;
    data.numOfQuestions = req.questionCount;

    unsigned int roomId = m_roomManager.createRoom(m_user, data);

    CreateRoomResponse response{ 1, roomId };

    Room& room = m_roomManager.getRoom(roomId);
    return {
        JsonResponsePacketSerializer::serializeResponse(response),
        m_handlerFactory.createRoomAdminRequestHandler(m_user, room)
    };
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo& requestInfo)
{
    m_roomManager.cleanEmptyInactiveRooms(); // 🔄 מחק חדרים לא פעילים וריקים

    GetRoomsResponse response;
    response.status = 1;
    response.rooms = m_roomManager.getRooms(); // ✅ יחזיר רק חדרים פעילים

    return {
        JsonResponsePacketSerializer::serializeResponse(response),
        std::make_unique<MenuRequestHandler>(*this)
    };
}




RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo)
{
    GetPlayersInRoomRequest req = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(requestInfo.buffer);

    if (!m_roomManager.doesRoomExist(req.roomId))
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Room does not exist" }), nullptr };

    auto& room = m_roomManager.getRoom(req.roomId);
    GetPlayersInRoomResponse response{ room.getAllUsers() };

    return { JsonResponsePacketSerializer::serializeResponse(response), nullptr };
}


RequestResult MenuRequestHandler::joinRoom(const RequestInfo& requestInfo)
{
    JoinRoomRequest req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.buffer);

    if (!m_roomManager.doesRoomExist(req.roomId))
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Room does not exist" }), nullptr };

    try {
        Room& room = m_roomManager.getRoom(req.roomId);

        // ✅ בדיקה חדשה – אם המשחק כבר התחיל, אל תאפשר להיכנס
        if (!room.isActive() || room.isGameStarted())
        {
            return {
                JsonResponsePacketSerializer::serializeResponse(ErrorResponse{
                    "Room is not joinable: game already started or room closed."
                }),
                nullptr
            };
        }

        room.addUser(m_user);
        JoinRoomResponse response{ 1 };
        return {
            JsonResponsePacketSerializer::serializeResponse(response),
            m_handlerFactory.createRoomMemberRequestHandler(m_user, room)
        };
    }
    catch (const std::exception& e) {
        return {
            JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() }),
            nullptr
        };
    }
}

