#pragma once
#include <string>

class LoggedUser
{
public:
    std::string getUsername() const;
    LoggedUser(const std::string& username) : m_userName(username) {}

private:
    std::string m_userName;

};