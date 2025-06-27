#pragma once
#include <string>

struct CreateRoomRequest
{
    std::string roomName;
    uint32_t maxUsers;
    uint32_t questionCount;
    uint32_t answerTimeout;
};
