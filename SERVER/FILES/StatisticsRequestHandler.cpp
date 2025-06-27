#include "StatisticsRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestCodes.h"
#include "StatisticsManager.h"
//#include "SqliteDatabase.h"

StatisticsRequestHandler::StatisticsRequestHandler(RequestHandlerFactory& factory, const LoggedUser& user)
    : m_handlerFactory(factory), m_user(user) {}

bool StatisticsRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == GET_PERSONAL_STATS_REQUEST || requestInfo.id == GET_GENERAL_STATS_REQUEST;
}

RequestResult StatisticsRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    if (requestInfo.id == GET_PERSONAL_STATS_REQUEST)
        return getPersonalStats();
    else if (requestInfo.id == GET_GENERAL_STATS_REQUEST)
        return getGeneralStats();

    return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid stats request" }), nullptr };
}

RequestResult StatisticsRequestHandler::getPersonalStats()
{
    StatisticsManager& statsManager = m_handlerFactory.getStatisticsManager();
    auto stats = statsManager.getUserPersonalStats(m_user.getUsername());

    return { JsonResponsePacketSerializer::serializeResponse(stats), m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult StatisticsRequestHandler::getGeneralStats()
{
    StatisticsManager& statsManager = m_handlerFactory.getStatisticsManager();

    GeneralStatsResponse response;
    response.status = 1;

    // topCorrectAnswers: vector<pair<string, int>>
    std::vector<PlayerResult> topCorrect = statsManager.getTopCorrectAnswers();
    for (const auto& result : topCorrect)
    {
        response.topCorrectAnswers.push_back(result);
    }

    // topWins: vector<pair<string, int>>
    std::vector<PlayerResult> topWins = statsManager.getTopWins();
    for (const auto& result : topWins)
    {
        response.topWins.push_back(result);
    }

    // topAccuracy: vector<tuple<string, int, int, float>>
    std::vector<PlayerResult> topAccuracy = statsManager.getTopAccuracy();
    for (const auto& result : topAccuracy)
    {
        response.topAccuracy.push_back(result);
    }

    return { JsonResponsePacketSerializer::serializeResponse(response), m_handlerFactory.createMenuRequestHandler(m_user) };
}
