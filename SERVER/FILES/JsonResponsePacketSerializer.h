#pragma once
#include <vector>
#include <string>
#include "Responses.h"
#include "json.hpp"
#include "Buffer.h"

using json = nlohmann::json;

// serializes server responses into byte buffers
class JsonResponsePacketSerializer
{
public:
    static Buffer serializeResponse(const LoginResponse& response);
    static Buffer serializeResponse(const SignupResponse& response);
    static Buffer serializeResponse(const ErrorResponse& response);
    static Buffer serializeResponse(const UsersListResponse& response);

    static Buffer serializeResponse(const GetRoomsResponse& response);
    static Buffer serializeResponse(const CreateRoomResponse& response);
    static Buffer serializeResponse(const JoinRoomResponse& response);
    static Buffer serializeResponse(const GetPlayersInRoomResponse& response);
    static Buffer serializeResponse(const LeaveRoomResponse& response);
    static Buffer serializeResponse(const CloseRoomResponse&);
    static Buffer serializeResponse(const GetRoomStateResponse& response);

    static Buffer serializeResponse(const StartGameResponse& response);
    static Buffer serializeResponse(const QuestionResponse& response);
    static Buffer serializeResponse(const SubmitAnswerResponse& resp);

    static Buffer serializeResponse(const GameResultsResponse& response);

    static Buffer serializeResponse(const UserPersonalStats& stats);
    static Buffer serializeResponse(const GeneralStatsResponse& stats);




};

