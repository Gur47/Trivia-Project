#pragma once

#include <vector>
#include "LoggedUser.h"
#include "RoomData.h"

class Room
{
public:
    Room(RoomData metadata);
    Room(); // default constructor


    void addUser(const LoggedUser& user);
    void removeUser(const LoggedUser& user);
    std::vector<std::string> getAllUsers() const;
    RoomData getRoomData() const;
    bool isActive() const;
    void closeRoom();
    std::vector<LoggedUser> getUsers() const;
    void startGame();
    bool isGameStarted() const;


private:
    RoomData m_metadata;
    std::vector<LoggedUser> m_users;
    bool m_isActive;
    bool m_isGameStarted;
};
