#include "GameRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "Responses.h"
#include "RequestCodes.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestHandlerFactory.h"     
#include "IDatabase.h"                 
#include "SqliteDatabase.h"

GameRequestHandler::GameRequestHandler(const LoggedUser& user, Room& room, GameManager& gameManager, RequestHandlerFactory& factory)
    : m_user(user), m_room(room), m_gameManager(gameManager), m_handlerFactory(factory) {}


bool GameRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == GET_QUESTION_REQUEST ||
        requestInfo.id == SUBMIT_ANSWER_REQUEST ||
        requestInfo.id == GET_GAME_RESULTS_REQUEST ||
        requestInfo.id == GET_ROOM_STATE_REQUEST;
}


RequestResult GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    switch (requestInfo.id)
    {
    case GET_QUESTION_REQUEST: return getQuestion(requestInfo);
    case SUBMIT_ANSWER_REQUEST: return submitAnswer(requestInfo);
    case GET_GAME_RESULTS_REQUEST: return getGameResults(requestInfo);
    case GET_ROOM_STATE_REQUEST:
    {
        GetRoomStateResponse response;
        response.status = 1;
        response.players = m_room.getAllUsers();
        response.questionCount = m_room.getRoomData().numOfQuestions;
        response.answerTimeout = m_room.getRoomData().timePerQuestion;
        response.maxPlayers = m_room.getRoomData().maxPlayers;
        response.isActive = m_room.isActive();
        response.isGameStarted = true; 

        return { JsonResponsePacketSerializer::serializeResponse(response), nullptr };
    }


    default:
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid game request" }), nullptr };
    }
}


RequestResult GameRequestHandler::getQuestion(const RequestInfo& requestInfo)
{
    Game* game = m_gameManager.getGame(m_room.getRoomData().id);
    if (!game)
    {
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Game not found" }), nullptr };
    }

    // בדיקה האם המשחק הסתיים
    if (!game->hasMoreQuestions(m_user))
    {
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Game is over" }), std::make_unique<GameRequestHandler>(*this) };
    }

    // אם השאלה כבר התחילה ובינתיים הזמן עבר – עבור לשאלה הבאה
    if (game->hasQuestionStarted() && game->isTimeUp())
    {
        IDatabase* db = m_handlerFactory.getDatabase();
        Question currentQ = game->getCurrentQuestionForUser(m_user);

        for (const auto& player : m_room.getAllUsers())
        {
            if (!game->hasPlayerAnswered(player))
            {
                db->insertDetailedStatistic(
                    player,
                    m_room.getRoomData().id,
                    currentQ.getQuestionText(),
                    "NO ANSWER",
                    currentQ.getCorrectAnswer(),
                    false,
                    (float)m_room.getRoomData().timePerQuestion
                );
            }
        }
        game->advanceToNextQuestion(); // שחקנים שלא ענו – נרשמת להם תשובה שגויה
    }

    // אם השאלה עוד לא התחילה – סמן כשחקן מוכן
    if (!game->hasQuestionStarted())
    {
        game->markPlayerReady(m_user);

        if (game->areAllPlayersReady())
        {
            game->startQuestionNow();
        }
        else
        {
            return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Please wait for all players or timeout." }), std::make_unique<GameRequestHandler>(*this) };
        }
    }

    // הצגת שאלה נוכחית
    Question question = game->getCurrentQuestionForUser(m_user);
    m_lastQuestionTime[m_user.getUsername()] = requestInfo.receivedTime;

    QuestionResponse response;
    response.status = 1;
    response.question = question.getQuestionText();
    response.answers = question.getShuffledAnswers();

    return { JsonResponsePacketSerializer::serializeResponse(response), std::make_unique<GameRequestHandler>(*this) };
}







//RequestResult GameRequestHandler::submitAnswer(const RequestInfo& requestInfo)
//{
//    auto req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfo.buffer);
//    Game* game = m_gameManager.getGame(m_room.getRoomData().id);
//    if (!game)
//    {
//        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Game not found" }), nullptr };
//    }
//
//    if (!game->hasQuestionStarted())
//    {
//        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Please wait for all players or timeout." }), std::make_unique<GameRequestHandler>(*this) };
//    }
//
//    // זמן תשובה
//    time_t now = requestInfo.receivedTime;
//    time_t last = m_lastQuestionTime[m_user.getUsername()];
//    float timeToAnswer = static_cast<float>(difftime(now, last));
//
//    // שליפת השאלה
//    Question currentQuestion = game->getCurrentQuestionForUser(m_user);
//    bool isCorrect = game->submitAnswer(m_user, req.answer);
//
//    // עדכון למסד הנתונים
//    IDatabase* db = m_handlerFactory.getDatabase();
//    db->insertDetailedStatistic(
//        m_user.getUsername(),
//        m_room.getRoomData().id,
//        currentQuestion.getQuestionText(),
//        req.answer,
//        currentQuestion.getCorrectAnswer(),
//        isCorrect,
//        timeToAnswer
//    );
//
//    // בדיקת סיום שאלה (כולם ענו או הזמן נגמר)
//    if (game->hasAllAnswered() || game->isTimeUp())
//    {
//        game->advanceToNextQuestion();
//    }
//
//    bool hasMore = game->hasMoreQuestions(m_user);
//
//    SubmitAnswerResponse resp;
//    resp.status = 1;
//    resp.correct = isCorrect;
//    resp.hasMoreQuestions = hasMore;
//
//    return { JsonResponsePacketSerializer::serializeResponse(resp), std::make_unique<GameRequestHandler>(*this) };
//}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& requestInfo)
{
    auto req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfo.buffer);
    Game* game = m_gameManager.getGame(m_room.getRoomData().id);
    if (!game)
    {
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Game not found" }), nullptr };
    }

    if (!game->hasQuestionStarted())
    {
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Please wait for all players or timeout." }), std::make_unique<GameRequestHandler>(*this) };
    }

    time_t now = requestInfo.receivedTime;
    time_t last = m_lastQuestionTime[m_user.getUsername()];
    float timeToAnswer = static_cast<float>(difftime(now, last));

    Question currentQuestion = game->getCurrentQuestionForUser(m_user);
    bool isCorrect = false;

    std::string submittedAnswer = req.answer.empty() ? "NO ANSWER" : req.answer;

    // גם תשובה ריקה תיחשב כאילו ענה (ולא תקריס)
    game->submitAnswer(m_user, submittedAnswer);

    // שמירה ל־DB
    IDatabase* db = m_handlerFactory.getDatabase();
    db->insertDetailedStatistic(
        m_user.getUsername(),
        m_room.getRoomData().id,
        currentQuestion.getQuestionText(),
        submittedAnswer,
        currentQuestion.getCorrectAnswer(),
        submittedAnswer == currentQuestion.getCorrectAnswer(),
        timeToAnswer
    );

    // אם כולם ענו או הזמן נגמר – עוברים שאלה
    if (game->hasAllAnswered() || game->isTimeUp())
    {
        game->advanceToNextQuestion();
    }

    SubmitAnswerResponse resp;
    resp.status = 1;
    resp.correct = isCorrect;
    resp.hasMoreQuestions = game->hasMoreQuestions(m_user);

    return { JsonResponsePacketSerializer::serializeResponse(resp), std::make_unique<GameRequestHandler>(*this) };
}


RequestResult GameRequestHandler::getGameResults(const RequestInfo& requestInfo)
{
    IDatabase* db = m_handlerFactory.getDatabase();
    SqliteDatabase* sqlite = dynamic_cast<SqliteDatabase*>(db);
    int roomId = m_room.getRoomData().id;

    Game* game = m_gameManager.getGame(roomId);
    if (!game)
    {
        return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Game not found" }), nullptr };
    }

    std::vector<std::tuple<std::string, int, float>> rawResults = sqlite->getGameResultsByRoom(roomId);

    GameResultsResponse response;
    response.status = 1;

    int totalAnswers = game->getNumQuestions();

    for (const auto& res : rawResults)
    {
        std::string username = std::get<0>(res);
        int correctAnswers = std::get<1>(res);
        float totalTime = std::get<2>(res);

        response.results.push_back(PlayerResult{ username, correctAnswers, totalTime });
    }

    // רק פעם אחת
    if (!game->isStatisticsUpdated())
    {
        game->markStatisticsUpdated(); // למנוע כפילות
        for (const auto& res : rawResults)
        {
            std::string username = std::get<0>(res);
            int correctAnswers = std::get<1>(res);
            float totalTime = std::get<2>(res);

            sqlite->updateUserStatistics(username, correctAnswers, totalAnswers, totalTime, 1);
        }
    }

    // לא למחוק את המשחק מיד – כי ייתכן ששחקן אחר עוד יבקש את התוצאות
    return { JsonResponsePacketSerializer::serializeResponse(response), m_handlerFactory.createMenuRequestHandler(m_user) };
}


