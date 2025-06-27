#pragma once
#include <vector>
#include <ctime>
#include <chrono>


// represents information about a request received from client
struct RequestInfo
{
    int id;
    std::vector<unsigned char> buffer;
    time_t receivedTime;
};