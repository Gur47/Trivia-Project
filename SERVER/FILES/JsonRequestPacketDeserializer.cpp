#include "JsonRequestPacketDeserializer.h"
#include "json.hpp"
#include "CreateRoomRequest.h"
#include "JoinRoomRequest.h"
#include "GetPlayersInRoomRequest.h"


using json = nlohmann::json;

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char>& buffer)
{
    std::string jsonStr(buffer.begin(), buffer.end());
    json j = json::parse(jsonStr);
    LoginRequest req;
    req.username = j["username"];
    req.password = j["password"];
    return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const std::vector<unsigned char>& buffer)
{
    std::string str(buffer.begin(), buffer.end());
    json j = json::parse(str);

    SignupRequest req;
    req.username = j["username"];
    req.password = j["password"];
    req.email = j["email"];

    return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    CreateRoomRequest req;
    req.roomName = j["roomName"];
    req.maxUsers = j["maxUsers"];
    req.questionCount = j["questionCount"];
    req.answerTimeout = j["answerTimeout"];
    return req;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    JoinRoomRequest req;
    req.roomId = j["roomId"];
    return req;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const std::vector<unsigned char>& buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    GetPlayersInRoomRequest req;
    req.roomId = j["roomId"];
    return req;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const std::vector<unsigned char>& buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    SubmitAnswerRequest req;
    req.answer = j["answer"];
    return req;
}
