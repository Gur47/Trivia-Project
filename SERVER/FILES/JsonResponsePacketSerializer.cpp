#include "JsonResponsePacketSerializer.h"
#include "Responses.h"
#include "Buffer.h"
#include "ResponseCodes.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse& response)
{
    json j;
    j["status"] = response.status;
    std::string str = j.dump();
    std::vector<unsigned char> rawBuffer;
    rawBuffer.push_back(LOGIN_RESPONSE);
    int size = str.size();
    rawBuffer.insert(rawBuffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    rawBuffer.insert(rawBuffer.end(), str.begin(), str.end());

    return Buffer(rawBuffer);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse& response)
{
    json j;
    j["status"] = response.status;
    std::string str = j.dump();

    std::vector<unsigned char> rawBuffer;
    rawBuffer.push_back(SIGNUP_RESPONSE);
    int size = str.size();
    rawBuffer.insert(rawBuffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    rawBuffer.insert(rawBuffer.end(), str.begin(), str.end());

    return Buffer(rawBuffer);  
    
}

Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& response)
{
    json j;
    j["message"] = response.message;
    std::string str = j.dump();

    std::vector<unsigned char> rawBuffer;
    rawBuffer.push_back(ERROR_RESPONSE);
    int size = str.size();
    rawBuffer.insert(rawBuffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    rawBuffer.insert(rawBuffer.end(), str.begin(), str.end());

    return Buffer(rawBuffer); 
}

Buffer JsonResponsePacketSerializer::serializeResponse(const UsersListResponse& response)
{
    json j;
    j["users"] = response.users;

    std::string str = j.dump();
    std::vector<unsigned char> vec;
    vec.push_back(USERS_LIST_RESPONSE);
    int len = str.size();
    vec.insert(vec.end(), reinterpret_cast<unsigned char*>(&len), reinterpret_cast<unsigned char*>(&len) + 4);
    vec.insert(vec.end(), str.begin(), str.end());

    return Buffer(vec);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
    json j;
    j["status"] = response.status;
    j["rooms"] = json::array();
    for (const auto& room : response.rooms)
    {
        j["rooms"].push_back({
            {"id", room.id},
            {"name", room.name},
            {"maxPlayers", room.maxPlayers},
            {"timePerQuestion", room.timePerQuestion},
            {"numOfQuestions", room.numOfQuestions}
            });
    }

    std::string str = j.dump();
    std::vector<unsigned char> rawBuffer;
    rawBuffer.push_back(GET_ROOMS_RESPONSE); 
    int size = str.size();
    rawBuffer.insert(rawBuffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    rawBuffer.insert(rawBuffer.end(), str.begin(), str.end());

    return Buffer(rawBuffer);
}


Buffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
    json j;
    j["status"] = response.status;
    j["roomId"] = response.roomId;

    std::string str = j.dump();
    std::vector<unsigned char> rawBuffer;
    rawBuffer.push_back(CREATE_ROOM_RESPONSE); 
    int size = str.size();
    rawBuffer.insert(rawBuffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    rawBuffer.insert(rawBuffer.end(), str.begin(), str.end());

    return Buffer(rawBuffer);
}


Buffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
    json j;
    j["status"] = response.status;

    std::string str = j.dump();
    std::vector<unsigned char> rawBuffer;
    rawBuffer.push_back(JOIN_ROOM_RESPONSE);
    int size = str.size();
    rawBuffer.insert(rawBuffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    rawBuffer.insert(rawBuffer.end(), str.begin(), str.end());

    return Buffer(rawBuffer);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    json j;
    j["users"] = response.users;

    std::string str = j.dump();
    std::vector<unsigned char> vec;
    vec.push_back(GET_PLAYERS_IN_ROOM_RESPONSE);
    int len = str.size();
    vec.insert(vec.end(), reinterpret_cast<unsigned char*>(&len), reinterpret_cast<unsigned char*>(&len) + 4);
    vec.insert(vec.end(), str.begin(), str.end());

    return Buffer(vec);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& response)
{
    nlohmann::json j;
    j["status"] = response.status;

    std::string jsonStr = j.dump();
    std::vector<unsigned char> buffer;
    buffer.push_back(LEAVE_ROOM_RESPONSE); // 9
    int size = jsonStr.size();
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());

    return Buffer(buffer); 
}

Buffer JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& res)
{
    nlohmann::json j;
    j["status"] = res.status;

    std::string jsonStr = j.dump();
    std::vector<unsigned char> buffer;
    buffer.push_back(CLOSE_ROOM_RESPONSE); 

    int size = jsonStr.size();
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());

    return Buffer(buffer);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& response)
{
    nlohmann::json j;
    j["status"] = response.status;
    j["players"] = response.players;
    j["questionCount"] = response.questionCount;
    j["answerTimeout"] = response.answerTimeout;
    j["maxPlayers"] = response.maxPlayers;
    j["isActive"] = response.isActive;
    j["isGameStarted"] = response.isGameStarted;

    std::string jsonStr = j.dump();
    std::vector<unsigned char> buffer;
    buffer.push_back(GET_ROOM_STATE_RESPONSE); // 11
    int len = jsonStr.size();
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char*>(&len), reinterpret_cast<unsigned char*>(&len) + 4);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());

    return Buffer(buffer);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& response)
{
    nlohmann::json j;
    j["status"] = response.status;

    std::string str = j.dump();
    std::vector<unsigned char> buffer;
    buffer.push_back(START_GAME_RESPONSE);
    int size = str.size();
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    buffer.insert(buffer.end(), str.begin(), str.end());

    return Buffer(buffer);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const QuestionResponse& response)
{
    nlohmann::json j;
    j["status"] = response.status;
    j["question"] = response.question;
    j["answers"] = response.answers;

    std::string str = j.dump();
    std::vector<unsigned char> buffer;
    buffer.push_back(GET_QUESTION_RESPONSE);
    int size = str.size();
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char*>(&size), reinterpret_cast<unsigned char*>(&size) + 4);
    buffer.insert(buffer.end(), str.begin(), str.end());

    return Buffer(buffer);
}


Buffer JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& resp)
{
    json j;
    j["status"] = resp.status;
    j["correct"] = resp.correct;
    j["hasMoreQuestions"] = resp.hasMoreQuestions;

    std::string str = j.dump();
    std::vector<unsigned char> bufferVec;
    bufferVec.push_back(SUBMIT_ANSWER_RESPONSE);
    int len = str.size();
    bufferVec.insert(bufferVec.end(), reinterpret_cast<unsigned char*>(&len), reinterpret_cast<unsigned char*>(&len) + 4);
    bufferVec.insert(bufferVec.end(), str.begin(), str.end());
    return Buffer(bufferVec);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GameResultsResponse& response)
{
    json j;
    j["status"] = response.status;
    j["results"] = json::array();

    for (const auto& result : response.results)
    {
        j["results"].push_back({
            {"username", result.username},
            {"correctAnswers", result.correctAnswers},
            { "totalTime", result.totalTime }
            });
    }

    std::string str = j.dump();
    std::vector<unsigned char> buffer;
    buffer.push_back(GET_GAME_RESULTS_RESPONSE);
    int len = str.size();
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char*>(&len), reinterpret_cast<unsigned char*>(&len) + 4);
    buffer.insert(buffer.end(), str.begin(), str.end());
    return Buffer(buffer);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const UserPersonalStats& stats)
{
    json j;
    j["username"] = stats.username;
    j["gamesPlayed"] = stats.gamesPlayed;
    j["correctAnswers"] = stats.correctAnswers;
    j["totalAnswers"] = stats.totalAnswers;
    j["averageTime"] = stats.averageTime;
    j["wins"] = stats.wins;

    std::string str = j.dump();
    std::vector<unsigned char> buf;
    buf.push_back(GET_PERSONAL_STATS_RESPONSE);  
    int len = str.size();
    buf.insert(buf.end(), reinterpret_cast<unsigned char*>(&len), reinterpret_cast<unsigned char*>(&len) + 4);
    buf.insert(buf.end(), str.begin(), str.end());

    return Buffer(buf);
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GeneralStatsResponse& stats)
{
    json j;
    j["status"] = stats.status;

    j["topCorrectAnswers"] = json::array();
    for (const auto& p : stats.topCorrectAnswers)
    {
        j["topCorrectAnswers"].push_back({
            {"username", p.username},
            {"value", static_cast<float>(p.correctAnswers)}  // זה מה שהלקוח מצפה לו
            });
    }

    j["topWins"] = json::array();
    for (const auto& p : stats.topWins)
    {
        j["topWins"].push_back({
            {"username", p.username},
            {"value", static_cast<float>(p.correctAnswers)}  // גם כאן value
            });
    }

    j["topAccuracy"] = json::array();
    for (const auto& p : stats.topAccuracy)
    {
        j["topAccuracy"].push_back({
            {"username", p.username},
            {"value", p.totalTime}  // כאן אחוז דיוק, בשדה value
            });
    }

    std::cout << j.dump(4) << std::endl;

    std::string str = j.dump();
    std::vector<unsigned char> buf;
    buf.push_back(GET_GENERAL_STATS_RESPONSE);
    int len = str.size();
    buf.insert(buf.end(), reinterpret_cast<unsigned char*>(&len), reinterpret_cast<unsigned char*>(&len) + 4);
    buf.insert(buf.end(), str.begin(), str.end());

    return Buffer(buf);
}
