#include "SqliteDatabase.h"
#include <iostream>
#include "sqlite3.h"
#include <algorithm>

SqliteDatabase::SqliteDatabase(const std::string& dbPath)
{
    if (sqlite3_open(dbPath.c_str(), &m_db) != SQLITE_OK)
    {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(m_db) << std::endl;
        m_db = nullptr;
    }
    else
    {
        const char* sql = "CREATE TABLE IF NOT EXISTS users (username TEXT , password TEXT NOT NULL, email TEXT, PRIMARY KEY(username, email));";   
        char* errMsg = nullptr;
        sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);
        if (errMsg) sqlite3_free(errMsg);

        resetDetailedStatistics();
        resetStatistics();
    }
}

SqliteDatabase::~SqliteDatabase()
{
    close();
}

void SqliteDatabase::close()
{
    if (m_db)
    {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool SqliteDatabase::doesUserExist(const std::string& username)
{
    std::string query = "SELECT 1 FROM users WHERE username = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_ROW;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
    std::string query = "SELECT 1 FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_ROW;
}

void SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
    std::string query = "INSERT INTO users (username, password, email) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(this->m_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::vector<std::string> SqliteDatabase::getAllUsernames()
{
    std::vector<std::string> users;
    std::string query = "SELECT username FROM users;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            users.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
    }

    sqlite3_finalize(stmt);
    return users;
}

std::vector<Question> SqliteDatabase::getQuestions(int amount)
{
    std::vector<Question> questions;
    std::string query = "SELECT question_text, correct_answer, wrong_answer1, wrong_answer2, wrong_answer3 "
        "FROM questions ORDER BY RANDOM() LIMIT ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, amount);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::string questionText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string correctAnswer = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string wrong1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string wrong2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            std::string wrong3 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

            questions.emplace_back(questionText, correctAnswer, std::vector<std::string>{wrong1, wrong2, wrong3});
        }
    }

    sqlite3_finalize(stmt);
    return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username)
{
    std::string query = "SELECT average_time FROM statistics WHERE username = ?";
    sqlite3_stmt* stmt;
    float result = 0.0f;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = static_cast<float>(sqlite3_column_double(stmt, 0));
        }
    }

    sqlite3_finalize(stmt);
    return result;
}


int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
    std::string query = "SELECT correct_answers FROM statistics WHERE username = ?";
    sqlite3_stmt* stmt;
    int result = 0;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = sqlite3_column_int(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
    std::string query = "SELECT total_answers FROM statistics WHERE username = ?";
    sqlite3_stmt* stmt;
    int result = 0;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = sqlite3_column_int(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
    std::string query = "SELECT games_played FROM statistics WHERE username = ?";
    sqlite3_stmt* stmt;
    int result = 0;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = sqlite3_column_int(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<std::string> SqliteDatabase::getHighScores()
{
    std::vector<std::string> topUsers;
    std::string query = "SELECT username FROM statistics ORDER BY correct_answers DESC LIMIT 5;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            topUsers.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
    }

    sqlite3_finalize(stmt);
    return topUsers;
}

void SqliteDatabase::insertStatisticsIfMissing(const std::string& username)
{
    std::string checkQuery = "SELECT 1 FROM statistics WHERE username = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, checkQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_ROW)
        {
            // משתמש לא קיים – נכניס אותו
            sqlite3_finalize(stmt);
            std::string insertQuery = "INSERT INTO statistics (username, correct_answers, total_answers, average_time, games_played) VALUES (?, 0, 0, 0.0, 0);";
            if (sqlite3_prepare_v2(m_db, insertQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
                sqlite3_step(stmt);
            }
        }
    }

    sqlite3_finalize(stmt);
}

void SqliteDatabase::updateUserStatistics(const std::string& username, int correctAdd, int totalAdd, float timeAdd, int gamesAdd)
{
    insertStatisticsIfMissing(username); // לוודא שהשורה קיימת

    std::string updateQuery =
        "UPDATE statistics SET "
        "correct_answers = correct_answers + ?, "
        "total_answers = total_answers + ?, "
        "average_time = "
        "CASE "
        "WHEN (total_answers + ?) > 0 THEN ((average_time * total_answers) + ?) / (total_answers + ?) "
        "ELSE 0 END, "
        "games_played = games_played + ? "
        "WHERE username = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, updateQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, correctAdd);
        sqlite3_bind_int(stmt, 2, totalAdd);
        sqlite3_bind_int(stmt, 3, totalAdd);         // עבור תנאי החלוקה
        sqlite3_bind_double(stmt, 4, timeAdd);
        sqlite3_bind_int(stmt, 5, totalAdd);         // שוב עבור תנאי החלוקה
        sqlite3_bind_int(stmt, 6, gamesAdd);
        sqlite3_bind_text(stmt, 7, username.c_str(), -1, SQLITE_STATIC);

        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
}


void SqliteDatabase::insertDetailedStatistic(const std::string& username, int roomId, const std::string& questionText,
    const std::string& givenAnswer, const std::string& correctAnswer,
    bool isCorrect, float timeTaken)
{
    std::string query =
        "INSERT INTO detailed_statistics (username, room_id, question_text, given_answer, correct_answer, is_correct, time_taken) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, roomId);
        sqlite3_bind_text(stmt, 3, questionText.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, givenAnswer.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, correctAnswer.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, isCorrect ? 1 : 0);
        sqlite3_bind_double(stmt, 7, timeTaken);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

std::vector<std::tuple<std::string, int, float>> SqliteDatabase::getGameResultsByRoom(int roomId)
{
    std::vector<std::tuple<std::string, int, float>> results;
    std::string query =
        "SELECT username, SUM(is_correct) AS correct_answers, SUM(time_taken) AS totalTime "
        "FROM detailed_statistics "
        "WHERE room_id = ? "
        "GROUP BY username "
        "ORDER BY correct_answers DESC, totalTime ASC;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, roomId);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int correctAnswers = sqlite3_column_int(stmt, 1);
            float time = static_cast<float>(sqlite3_column_double(stmt, 2));
            results.emplace_back(username, correctAnswers, time);
        }

        sqlite3_finalize(stmt);
    }

    return results;
}

void SqliteDatabase::resetDetailedStatistics()
{
    const char* sql = "DELETE FROM detailed_statistics;";
    char* errMsg = nullptr;

    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to reset detailed_statistics: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "[DB] detailed_statistics table cleared.\n";
    }
}


std::unordered_map<std::string, int> SqliteDatabase::calculateWinsPerUser()
{
    std::unordered_map<std::string, int> wins;

    // נשלוף את כל החדרים ששוחקו
    std::string query =
        "SELECT room_id FROM detailed_statistics "
        "GROUP BY room_id";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int roomId = sqlite3_column_int(stmt, 0);

            // עכשיו נחשב מי ניצח באותו חדר לפי תנאים:
            std::string innerQuery =
                "SELECT username, SUM(is_correct) AS correct, SUM(time_taken) AS total_time "
                "FROM detailed_statistics "
                "WHERE room_id = ? "
                "GROUP BY username "
                "ORDER BY correct DESC, total_time ASC";

            sqlite3_stmt* innerStmt;
            if (sqlite3_prepare_v2(m_db, innerQuery.c_str(), -1, &innerStmt, nullptr) == SQLITE_OK)
            {
                sqlite3_bind_int(innerStmt, 1, roomId);

                std::vector<std::tuple<std::string, int, float>> roomResults;
                int maxCorrect = -1;
                float bestTime = -1;

                while (sqlite3_step(innerStmt) == SQLITE_ROW)
                {
                    std::string username = reinterpret_cast<const char*>(sqlite3_column_text(innerStmt, 0));
                    int correct = sqlite3_column_int(innerStmt, 1);
                    float time = static_cast<float>(sqlite3_column_double(innerStmt, 2));
                    roomResults.emplace_back(username, correct, time);

                    if (correct > maxCorrect || (correct == maxCorrect && time < bestTime))
                    {
                        maxCorrect = correct;
                        bestTime = time;
                    }
                }

                // אחרי שאספנו את כולם – נזהה מי מהמובילים עומד בתנאים
                for (const auto& res : roomResults)
                {
                    if (std::get<1>(res) == maxCorrect &&
                        fabs(std::get<2>(res) - bestTime) < 0.01f)
                    {
                        wins[std::get<0>(res)]++;
                    }
                }

                sqlite3_finalize(innerStmt);
            }
        }

        sqlite3_finalize(stmt);
    }

    return wins;
}

UserPersonalStats SqliteDatabase::getUserPersonalStats(const std::string& username)
{
    UserPersonalStats stats{ username, 0, 0, 0, 0.0f, 0 };

    // שלוף נתונים מהטבלה הרגילה
    std::string query =
        "SELECT games_played, correct_answers, total_answers, average_time "
        "FROM statistics WHERE username = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            stats.gamesPlayed = sqlite3_column_int(stmt, 0);
            stats.correctAnswers = sqlite3_column_int(stmt, 1);
            stats.totalAnswers = sqlite3_column_int(stmt, 2);
            stats.averageTime = static_cast<float>(sqlite3_column_double(stmt, 3));
        }

        sqlite3_finalize(stmt);
    }

    // שלוף כמות נצחונות
    auto allWins = calculateWinsPerUser();
    // תחליף בטוח:
    if (allWins.find(username) != allWins.end())
    {
        stats.wins = allWins[username];
    }


    return stats;
}

std::vector<std::pair<std::string, int>> SqliteDatabase::getTopCorrectAnswers()
{
    std::vector<std::pair<std::string, int>> result;
    std::string query = "SELECT username, correct_answers FROM statistics ORDER BY correct_answers DESC LIMIT 5;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result.emplace_back(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                sqlite3_column_int(stmt, 1)
            );
        }
        sqlite3_finalize(stmt);
    }

    return result;
}

std::vector<std::pair<std::string, int>> SqliteDatabase::getTopWins()
{
    std::vector<std::pair<std::string, int>> result;
    auto wins = calculateWinsPerUser();

    std::vector<std::pair<std::string, int>> all(wins.begin(), wins.end());
    std::sort(all.begin(), all.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
        });

    for (int i = 0; i < std::min(5, (int)all.size()); i++)
    {
        result.push_back(all[i]);
    }

    return result;
}

std::vector<std::tuple<std::string, int, int, float>> SqliteDatabase::getTopAccuracy()
{
    std::vector<std::tuple<std::string, int, int, float>> result;
    std::string query =
        "SELECT username, correct_answers, total_answers, "
        "CAST(correct_answers AS REAL) / NULLIF(total_answers, 0) AS accuracy "
        "FROM statistics WHERE total_answers > 0 "
        "ORDER BY accuracy DESC LIMIT 5;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result.emplace_back(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                sqlite3_column_int(stmt, 1),
                sqlite3_column_int(stmt, 2),
                static_cast<float>(sqlite3_column_double(stmt, 3))
            );
        }

        sqlite3_finalize(stmt);
    }

    return result;
}

void SqliteDatabase::resetStatistics()
{
    const char* sql = "DELETE FROM statistics;";
    char* errMsg = nullptr;

    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to reset statistics: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "[DB] statistics table cleared.\n";
    }
}
