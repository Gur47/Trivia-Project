#include "GameManager.h"

GameManager::GameManager() {}

Game& GameManager::createGame(int roomId, const std::vector<LoggedUser>& users, const std::vector<Question>& questions, int timePerQuestion)
{
    Game newGame(users, questions, timePerQuestion);
    m_games[roomId] = newGame;
    return m_games[roomId];
}

Game* GameManager::getGame(int roomId)
{
    auto it = m_games.find(roomId);
    return (it != m_games.end()) ? &it->second : nullptr;
}

void GameManager::deleteGame(int roomId)
{
    m_games.erase(roomId);
}
