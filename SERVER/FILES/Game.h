#pragma once
#include "Question.h"
#include "LoggedUser.h"
#include <vector>
#include <unordered_map>

class Game
{
public:
    Game(const std::vector<LoggedUser>& players, const std::vector<Question>& questions, int timePerQuestion);
    Game() = default;

    Question getCurrentQuestionForUser(const LoggedUser& user);
    //void moveToNextQuestion(const LoggedUser& user);
    bool hasMoreQuestions(const LoggedUser& user) const;
    bool submitAnswer(const LoggedUser& user, const std::string& answer);

    bool hasAllAnswered() const;
    bool isTimeUp() const;
    void resetAnswersForNextQuestion();
    bool hasQuestionStarted() const;
    void markPlayerReady(const LoggedUser& user);
    bool areAllPlayersReady() const;
    void startQuestionNow();
    void advanceToNextQuestion();
    bool hasPlayerAnswered(const std::string& username) const;

    int getNumQuestions() const;
    bool isStatisticsUpdated() const { return m_statisticsUpdated; }
    void markStatisticsUpdated() { m_statisticsUpdated = true; }

private:
    std::vector<LoggedUser> m_players;
    std::vector<Question> m_questions;
    //std::unordered_map<std::string, int> m_userQuestionIndex;
    std::unordered_map<std::string, bool> m_hasAnswered;
    std::unordered_map<std::string, bool> m_readyForQuestion;  // ✅ חדש
    int m_currentQuestionIndex;
    int m_timePerQuestion;
    time_t m_questionStartTime;
    bool m_gameOver = false;
    bool m_statisticsUpdated = false;


};
