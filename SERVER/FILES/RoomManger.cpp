#include "RoomManager.h"
#include <stdexcept>

unsigned int RoomManager::generateRoomId() {
    static unsigned int id = 1;
    while (m_rooms.find(id) != m_rooms.end()) {
        ++id;
    }
    return id;
}

void RoomManager::createRoom(const LoggedUser& user, const RoomData& data) {
    RoomData newData = data;
    newData.id = generateRoomId();
    Room room(newData); // יצירת Room עם RoomData
    room.addUser(user);
    m_rooms[newData.id] = room;
}

void RoomManager::deleteRoom(unsigned int roomId) {
    m_rooms.erase(roomId);
}

RoomData RoomManager::getRoomData(unsigned int roomId) const {
    auto it = m_rooms.find(roomId);
    if (it == m_rooms.end()) {
        throw std::runtime_error("Room not found");
    }
    return it->second.getRoomData();
}

std::vector<RoomData> RoomManager::getRooms() const {
    std::vector<RoomData> rooms;
    for (const auto& pair : m_rooms) {
        rooms.push_back(pair.second.getRoomData());
    }
    return rooms;
}

void RoomManager::joinRoom(const LoggedUser& user, unsigned int roomId) {
    auto it = m_rooms.find(roomId);
    if (it == m_rooms.end()) {
        throw std::runtime_error("Room not found");
    }
    it->second.addUser(user);
}

void RoomManager::leaveRoom(const LoggedUser& user, unsigned int roomId) {
    auto it = m_rooms.find(roomId);
    if (it == m_rooms.end()) {
        throw std::runtime_error("Room not found");
    }
    it->second.removeUser(user);
}

std::vector<LoggedUser> RoomManager::getPlayersInRoom(unsigned int roomId) const {
    auto it = m_rooms.find(roomId);
    if (it == m_rooms.end()) {
        throw std::runtime_error("Room not found");
    }
    return it->second.getAllUsers();
}