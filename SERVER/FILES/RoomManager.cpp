#include "RoomManager.h"
#include <stdexcept>
#include <iostream>
#include "Game.h"
#include "SqliteDatabase.h"

RoomManager::RoomManager(IDatabase* database)
    : m_nextRoomId(1), m_database(database) {}


unsigned int RoomManager::createRoom(const LoggedUser& admin, const RoomData& metadata)
{
    RoomData newMetadata = metadata;
    newMetadata.id = m_nextRoomId++;

    Room room(newMetadata);
    room.addUser(admin);

    m_rooms[newMetadata.id] = room;
    return room.getRoomData().id;

}

void RoomManager::deleteRoom(unsigned int roomId)
{
    m_rooms.erase(roomId);
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> activeRooms;

    for (const auto& pair : m_rooms)
    {
        const Room& room = pair.second;
        if (room.isActive() && !room.isGameStarted())
        {
            activeRooms.push_back(room.getRoomData());
        }
    }

    return activeRooms;
}

Room& RoomManager::getRoom(unsigned int roomId)
{
    auto it = m_rooms.find(roomId);
    if (it == m_rooms.end()) {
        throw std::runtime_error("Room not found");
    }
    return it->second;
}


bool RoomManager::doesRoomExist(unsigned int roomId) const
{
    return m_rooms.find(roomId) != m_rooms.end();
}

void RoomManager::cleanEmptyInactiveRooms()
{
    for (auto it = m_rooms.begin(); it != m_rooms.end(); )
    {
        const Room& room = it->second;

        bool shouldDelete = (room.getAllUsers().empty() &&
            (!room.isActive() || room.isGameStarted()));

        if (shouldDelete)
        {
            std::cout << "[DEBUG] Deleting roomId: " << it->first << std::endl;
            it = m_rooms.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void RoomManager::startGame(unsigned int roomId)
{
    if (!doesRoomExist(roomId))
        throw std::runtime_error("Cannot start game: room doesn't exist");

    int questionCount = m_rooms[roomId].getRoomData().numOfQuestions;
    int timePerQuestion = m_rooms[roomId].getRoomData().timePerQuestion;

    std::vector<Question> allQuestions = m_database->getQuestions(1000);
    if (questionCount > allQuestions.size())
        throw std::runtime_error("Not enough questions in the database.");

    std::vector<Question> selectedQuestions(allQuestions.begin(), allQuestions.begin() + questionCount);

    Game game(m_rooms[roomId].getUsers(), selectedQuestions, timePerQuestion);
    m_activeGames.insert({ roomId, game });

    m_rooms[roomId].startGame();
}




Game& RoomManager::getGame(unsigned int roomId)
{
    return m_activeGames.at(roomId);
}

IDatabase* RoomManager::getDatabase() const
{
    return m_database;
}
