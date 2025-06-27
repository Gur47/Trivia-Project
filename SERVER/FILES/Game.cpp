#include "Game.h"
#include <stdexcept>
#include <ctime>

Game::Game(const std::vector<LoggedUser>& players, const std::vector<Question>& questions, int timePerQuestion)
    : m_players(players), m_questions(questions), m_timePerQuestion(timePerQuestion), m_currentQuestionIndex(0)
{
    for (const auto& user : m_players)
    {
        //m_userQuestionIndex[user.getUsername()] = 0;
        m_hasAnswered[user.getUsername()] = false;
        m_readyForQuestion[user.getUsername()] = false;
    }
    m_questionStartTime = std::time(nullptr);
}
Question Game::getCurrentQuestionForUser(const LoggedUser& user)
{
    if (m_currentQuestionIndex >= m_questions.size())
        throw std::runtime_error("No more questions");

    return m_questions[m_currentQuestionIndex];
}


//void Game::moveToNextQuestion(const LoggedUser& user)
//{
//    m_userQuestionIndex[user.getUsername()]++;
//    m_hasAnswered[user.getUsername()] = false;
//}

bool Game::hasMoreQuestions(const LoggedUser& user) const
{
    return m_currentQuestionIndex < m_questions.size();
}


bool Game::submitAnswer(const LoggedUser& user, const std::string& answer)
{
    Question q = getCurrentQuestionForUser(user);
    bool correct = (q.getCorrectAnswer() == answer);
    m_hasAnswered[user.getUsername()] = true;
    return correct;
}


bool Game::hasAllAnswered() const
{
    for (const auto& pair : m_hasAnswered)
    {
        if (!pair.second)
            return false;
    }
    return true;
}

bool Game::isTimeUp() const
{
    int elapsed = static_cast<int>(std::time(nullptr) - m_questionStartTime);
    return elapsed >= m_timePerQuestion;
}

void Game::resetAnswersForNextQuestion()
{
    for (auto& pair : m_hasAnswered)
    {
        pair.second = false;
    }
    for (auto& pair : m_readyForQuestion)
    {
        pair.second = false;
    }

    m_currentQuestionIndex++;
    m_questionStartTime = 0; // לא נחשב שאלה עד שכולם יבקשו אותה  
}

bool Game::hasQuestionStarted() const
{
    return m_questionStartTime != 0;
}

void Game::markPlayerReady(const LoggedUser& user)
{
    m_readyForQuestion[user.getUsername()] = true;
}

bool Game::areAllPlayersReady() const
{
    for (const auto& pair : m_readyForQuestion)
    {
        if (!pair.second)
            return false;
    }
    return true;
}


void Game::startQuestionNow()
{
    m_questionStartTime = time(nullptr);
}

void Game::advanceToNextQuestion()
{
    resetAnswersForNextQuestion(); // אפס תשובות וסמן שהשאלה הסתיימה

    if (m_currentQuestionIndex >= m_questions.size())
    {
        m_gameOver = true;
    }
}

bool Game::hasPlayerAnswered(const std::string& username) const
{
    auto it = m_hasAnswered.find(username);
    return it != m_hasAnswered.end() && it->second;
}

int Game::getNumQuestions() const
{
    return static_cast<int>(m_questions.size());
}

