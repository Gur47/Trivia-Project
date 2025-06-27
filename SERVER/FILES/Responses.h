#pragma once
#include <string>
#include "RoomData.h"
#include <vector>
#include <tuple>



// response for login request
struct LoginResponse
{
    unsigned int status;
};

// response for signup request
struct SignupResponse
{
    unsigned int status;
};

// response for error
struct ErrorResponse
{
    std::string message;
};

struct UsersListResponse
{
    std::vector<std::string> users;
};


struct GetRoomsResponse
{
    unsigned int status;
    std::vector<RoomData> rooms;
};

struct CreateRoomResponse
{
    unsigned int status;
    unsigned int roomId;
};

struct JoinRoomResponse
{
    unsigned int status;
};

struct GetPlayersInRoomResponse
{
    std::vector<std::string> users;
};

struct LeaveRoomResponse
{
    unsigned int status;
};

struct CloseRoomResponse
{
    unsigned int status;
};

struct GetRoomStateResponse
{
    unsigned int status;
    std::vector<std::string> players;
    unsigned int questionCount;
    unsigned int answerTimeout;
    unsigned int maxPlayers;
    bool isActive;
    bool isGameStarted;
};

struct StartGameResponse
{
    unsigned int status;
};

struct QuestionResponse
{
    unsigned int status;
    std::string question;
    std::vector<std::string> answers;
};

struct SubmitAnswerResponse
{
    unsigned int status;
    bool correct;
    bool hasMoreQuestions;
};

struct PlayerResult
{
    std::string username;
    int correctAnswers;
    float totalTime;
};

struct GameResultsResponse
{
    unsigned int status;
    std::vector<PlayerResult> results;
};

struct UserPersonalStats
{
    std::string username;
    int gamesPlayed;
    int correctAnswers;
    int totalAnswers;
    float averageTime;
    int wins;
};

struct GeneralStatsResponse
{
    unsigned int status;
    std::vector<PlayerResult> topCorrectAnswers;
    std::vector<PlayerResult> topWins;
    std::vector<PlayerResult> topAccuracy;
};

