#include "StatisticsManager.h"
#include "SqliteDatabase.h"

StatisticsManager::StatisticsManager(IDatabase* db)
    : m_database(db) {}

float StatisticsManager::getPlayerAverageTime(const std::string& username)
{
    return m_database->getPlayerAverageAnswerTime(username);
}

int StatisticsManager::getNumOfCorrectAnswers(const std::string& username)
{
    return m_database->getNumOfCorrectAnswers(username);
}

int StatisticsManager::getNumOfTotalAnswers(const std::string& username)
{
    return m_database->getNumOfTotalAnswers(username);
}

int StatisticsManager::getNumOfPlayerGames(const std::string& username)
{
    return m_database->getNumOfPlayerGames(username);
}

std::vector<std::string> StatisticsManager::getHighScores()
{
    return m_database->getHighScores();
}

// 🔹 מימוש של הסטטיסטיקות החדשות:

UserPersonalStats StatisticsManager::getUserPersonalStats(const std::string& username)
{
    return static_cast<SqliteDatabase*>(m_database)->getUserPersonalStats(username);
}

std::vector<PlayerResult> StatisticsManager::getTopCorrectAnswers()
{
    auto raw = static_cast<SqliteDatabase*>(m_database)->getTopCorrectAnswers();
    std::vector<PlayerResult> converted;
    for (const auto& p : raw)
    {
        converted.push_back(PlayerResult{ p.first, p.second, 0 }); // totalTime = 0
    }
    return converted;
}

std::vector<PlayerResult> StatisticsManager::getTopWins()
{
    auto raw = static_cast<SqliteDatabase*>(m_database)->getTopWins();
    std::vector<PlayerResult> converted;
    for (const auto& p : raw)
    {
        converted.push_back(PlayerResult{ p.first, p.second, 0 }); // totalTime = 0
    }
    return converted;
}

std::vector<PlayerResult> StatisticsManager::getTopAccuracy()
{
    auto raw = static_cast<SqliteDatabase*>(m_database)->getTopAccuracy();
    std::vector<PlayerResult> converted;
    for (const auto& t : raw)
    {
        converted.push_back(PlayerResult{
            std::get<0>(t), // username
            std::get<1>(t), // correct
            std::get<3>(t)  // accuracy (שמור ב־totalTime רק כדי לשלוח ללקוח, נשתמש בו ב־XAML)
            });
    }
    return converted;
}

