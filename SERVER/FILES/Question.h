#pragma once
#include <string>
#include <vector>

class Question
{
public:
    Question(const std::string& question, const std::string& correctAnswer, const std::vector<std::string>& wrongAnswers);

    std::string getQuestionText() const;
    std::string getCorrectAnswer() const;
    std::vector<std::string> getShuffledAnswers() const;


private:
    std::string m_question;
    std::string m_correctAnswer;
    std::vector<std::string> m_wrongAnswers;
};
