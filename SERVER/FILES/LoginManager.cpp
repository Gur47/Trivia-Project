#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* db) : m_database(db) {}

bool LoginManager::login(const std::string& username, const std::string& password)
{
    if (!m_database->doesUserExist(username) || !m_database->doesPasswordMatch(username, password))
        return false;

    m_loggedInUsers.insert(username);
    return true;
}

void LoginManager::logout(const std::string& username)
{
    m_loggedInUsers.erase(username);
}

bool LoginManager::isUserLoggedIn(const std::string& username)
{
    return m_loggedInUsers.find(username) != m_loggedInUsers.end();
}

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    if (m_database->doesUserExist(username))
        return false;

    m_database->addNewUser(username, password, email);
    m_loggedInUsers.insert(username);
    return true;
}

std::vector<std::string> LoginManager::getAllUsers()
{
    return m_database->getAllUsernames();
}

IDatabase* LoginManager::getDatabase()
{
    return m_database;
}
