#pragma once
#include "IDatabase.h"
#include <set>
#include <string>

class LoginManager
{
public:
    LoginManager(IDatabase* db);
    bool login(const std::string& username, const std::string& password);
    void logout(const std::string& username);
    bool isUserLoggedIn(const std::string& username);
    bool signup(const std::string& username, const std::string& password, const std::string& email);
    std::vector<std::string> getAllUsers();
    IDatabase* getDatabase();




private:
    IDatabase* m_database;
    std::set<std::string> m_loggedInUsers;
};
