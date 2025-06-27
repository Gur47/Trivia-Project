#pragma once

#include <map>
#include <vector>
#include "Room.h"
#include "Game.h"
#include "IDatabase.h"


class RoomManager
{
public:
    RoomManager(IDatabase* database);

    unsigned int createRoom(const LoggedUser& admin, const RoomData& metadata);
    void deleteRoom(unsigned int roomId);
    std::vector<RoomData> getRooms() const;
    Room& getRoom(unsigned int roomId);
    bool doesRoomExist(unsigned int roomId) const;
    void cleanEmptyInactiveRooms();
    void startGame(unsigned int roomId);
    Game& getGame(unsigned int roomId);
    IDatabase* getDatabase() const;



private:
    std::map<unsigned int, Room> m_rooms;
    unsigned int m_nextRoomId;
    std::map<unsigned int, Game> m_activeGames;
    IDatabase* m_database;


};
