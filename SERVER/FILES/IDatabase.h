#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Question.h"



class IDatabase
{
public:
    virtual bool doesUserExist(const std::string& username) = 0;
    virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
    virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;
    virtual ~IDatabase() = default;
    virtual std::vector<std::string> getAllUsernames() = 0;
    virtual std::vector<Question> getQuestions(int amount) = 0;

    virtual float getPlayerAverageAnswerTime(const std::string& username) = 0;
    virtual int getNumOfCorrectAnswers(const std::string& username) = 0;
    virtual int getNumOfTotalAnswers(const std::string& username) = 0;
    virtual int getNumOfPlayerGames(const std::string& username) = 0;
    virtual std::vector<std::string> getHighScores() = 0;
    //virtual void updateUserStatistics(const std::string& username, int correctAdd, int totalAdd, float timeAdd, int gamesAdd) = 0;
    virtual void insertDetailedStatistic(const std::string& username, int roomId, const std::string& questionText,
        const std::string& givenAnswer, const std::string& correctAnswer,
        bool isCorrect, float timeTaken) = 0;

};
