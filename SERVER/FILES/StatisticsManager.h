#pragma once
#include "IDatabase.h"
#include "Responses.h"
#include <string>
#include <vector>
#include <tuple>
#include <utility>

class StatisticsManager
{
public:
    StatisticsManager(IDatabase* db);

    float getPlayerAverageTime(const std::string& username);
    int getNumOfCorrectAnswers(const std::string& username);
    int getNumOfTotalAnswers(const std::string& username);
    int getNumOfPlayerGames(const std::string& username);
    std::vector<std::string> getHighScores();

    // סטטיסטיקה אישית
    UserPersonalStats getUserPersonalStats(const std::string& username);

    // סטטיסטיקות כלליות
    std::vector<PlayerResult> getTopCorrectAnswers();
    std::vector<PlayerResult> getTopWins();
    std::vector<PlayerResult> getTopAccuracy();

private:
    IDatabase* m_database;
};
