#include "Room.h"
#include <iostream>

Room::Room(RoomData metadata)
    : m_metadata(metadata), m_isActive(true), m_isGameStarted(false) {}

Room::Room()
    : m_metadata{ 0, "", 0, 0, 0 }, m_isActive(false), m_isGameStarted(false) {}


void Room::addUser(const LoggedUser& user)
{
    if (!m_isActive) {
        throw std::runtime_error("Cannot add user to inactive room");
    }
    if (m_users.size() >= m_metadata.maxPlayers) {
        throw std::runtime_error("Room is full");
    }

    m_users.push_back(user);
}

void Room::removeUser(const LoggedUser& user)
{
    m_users.erase(
        std::remove_if(m_users.begin(), m_users.end(),
            [&](const LoggedUser& u) {
                return u.getUsername() == user.getUsername();
            }),
        m_users.end()
    );
}

std::vector<std::string> Room::getAllUsers() const
{
    std::vector<std::string> usernames;
    for (const auto& user : m_users)
    {
        usernames.push_back(user.getUsername());
    }
    return usernames;
}

RoomData Room::getRoomData() const
{
    return m_metadata;
}

bool Room::isActive() const
{
    return m_isActive;
}

void Room::closeRoom()
{
    m_isActive = false;
}

std::vector<LoggedUser> Room::getUsers() const
{
    return m_users;
}

void Room::startGame()
{
    m_isGameStarted = true;
}

bool Room::isGameStarted() const
{
    return m_isGameStarted;
}