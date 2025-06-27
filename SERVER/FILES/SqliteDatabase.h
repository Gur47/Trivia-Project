#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <string>
#include "Question.h" 
#include <tuple>
#include <unordered_map>
#include <string>
#include "Responses.h"




class SqliteDatabase : public IDatabase
{
public:
    SqliteDatabase(const std::string& dbPath = "trivia.db");
    ~SqliteDatabase();

    bool doesUserExist(const std::string& username) override;
    bool doesPasswordMatch(const std::string& username, const std::string& password) override;
    void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
    std::vector<std::string> getAllUsernames() override;
    std::vector<Question> getQuestions(int amount);

    float getPlayerAverageAnswerTime(const std::string& username) override;
    int getNumOfCorrectAnswers(const std::string& username) override;
    int getNumOfTotalAnswers(const std::string& username) override;
    int getNumOfPlayerGames(const std::string& username) override;
    std::vector<std::string> getHighScores() override;
    void insertStatisticsIfMissing(const std::string& username);
    void updateUserStatistics(const std::string& username, int correctAdd, int totalAdd, float timeAdd, int gamesAdd);
    void insertDetailedStatistic(const std::string& username, int roomId, const std::string& questionText,
    const std::string& givenAnswer, const std::string& correctAnswer,
    bool isCorrect, float timeTaken) override;

    std::vector<std::tuple<std::string, int, float>> getGameResultsByRoom(int roomId);

    void resetDetailedStatistics();  
    void resetStatistics();

    std::unordered_map<std::string, int> calculateWinsPerUser();
    UserPersonalStats getUserPersonalStats(const std::string& username);
    std::vector<std::pair<std::string, int>> getTopCorrectAnswers();
    std::vector<std::pair<std::string, int>> getTopWins();
    std::vector<std::tuple<std::string, int, int, float>> getTopAccuracy(); // שם, נכונות, סה"כ, יחס






private:
    sqlite3* m_db;
    bool open();
    void close();
};