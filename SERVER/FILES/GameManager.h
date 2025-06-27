#pragma once
#include <map>
#include <vector>
#include "Game.h"
#include "LoggedUser.h"
#include "Question.h"

class GameManager
{
public:
    GameManager();
    Game& createGame(int roomId, const std::vector<LoggedUser>& users, 
    const std::vector<Question>& questions, int timePerQuestion);
    Game* getGame(int roomId);
    void deleteGame(int roomId);

private:
    std::map<int, Game> m_games;
    //IDatabase* m_database;

};
