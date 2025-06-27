#include "Question.h"
#include <algorithm>
#include <random>

Question::Question(const std::string& question, const std::string& correctAnswer, const std::vector<std::string>& wrongAnswers)
    : m_question(question), m_correctAnswer(correctAnswer), m_wrongAnswers(wrongAnswers) {}

std::string Question::getQuestionText() const
{
    return m_question;
}

std::string Question::getCorrectAnswer() const
{
    return m_correctAnswer;
}

std::vector<std::string> Question::getShuffledAnswers() const
{
    std::vector<std::string> allAnswers = m_wrongAnswers;
    allAnswers.push_back(m_correctAnswer);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allAnswers.begin(), allAnswers.end(), g);

    return allAnswers;
}
