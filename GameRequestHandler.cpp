#include "GameRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "structs.h"

GameRequestHandler::GameRequestHandler(
    Game& game,
    LoggedUser user,
    GameManager& gameManager,
    RequestHandlerFactory& handlerFactory
)
    : m_game(game),
    m_user(user),
    m_gameManager(gameManager),
    m_handlerFactory(handlerFactory)
{
}

/*
* If the client closes the window during a game, the handler is
* destroyed. (Meant that he left the game)
*/
GameRequestHandler::~GameRequestHandler()
{
    m_game.removePlayer(m_user.username);
}

bool GameRequestHandler::isRequestRelevant(
    const RequestInfo& requestInfo
) const
{
    return requestInfo.messageCode == RequestCode::LEAVE_GAME_REQ ||
        requestInfo.messageCode == RequestCode::GET_QUESTION_REQ ||
        requestInfo.messageCode == RequestCode::SUBMIT_ANSWER_REQ ||
        requestInfo.messageCode == RequestCode::GET_GAME_RESULT_REQ;
}

RequestResult GameRequestHandler::handleRequest(
    const RequestInfo& requestInfo
)
{
    if (requestInfo.messageCode == RequestCode::GET_QUESTION_REQ)
    {
        return getQuestion(requestInfo);
    }
    else if (requestInfo.messageCode == RequestCode::SUBMIT_ANSWER_REQ)
    {
        return submitAnswer(requestInfo);
    }
    else if (requestInfo.messageCode == RequestCode::GET_GAME_RESULT_REQ)
    {
        return getGameResults(requestInfo);
    }
    else if (requestInfo.messageCode == RequestCode::LEAVE_GAME_REQ)
    {
        return leaveGame(requestInfo);
    }

    return RequestResult{ std::vector<unsigned char>(), nullptr };
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo& requestInfo)
{
    Question q = m_game.getQuestionForUser(m_user.username);

    GetQuestionResponse res;
    if (q.question.empty())
    {
        res.status = 0;
        res.question = "";
    }
    else
    {
        res.status = 1;
        res.question = q.question;
        for (unsigned int i = 0; i < q.possibleAnswers.size(); i++)
        {
            res.answers[i] = q.possibleAnswers[i];
        }
    }

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeGetQuestionResponse(res);
    result.newHandler = this;

    return result;
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& requestInfo)
{
    SubmitAnswerRequest req =
        JsonRequestPacketDeserializer::deserializerSubmitAnswerRequest(
            requestInfo
        );

    unsigned int correctId = m_game.submitAnswer(
            m_user.username, 
            req.answerId, 
            requestInfo.receivalTime
        );

    SubmitAnswerResponse res;
    res.status = correctId == 0 ? 0 : 1;
    res.correctAnswerId = correctId;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeSubmitAnswerResponse(res);
    result.newHandler = this;

    return result;
}

/* 
* Results are returned only after all players finished.
* Until then, the client keeps showing the waiting screen.
*/
RequestResult GameRequestHandler::getGameResults(const RequestInfo& requestInfo)
{
    GetGameResultsResponse res;

    if (!m_game.isFinished())
    {
        res.status = 0;
    }
    else
    {
        res.status = 1;

        auto results = m_game.getResults();
        for (const auto& pair : results)
        {
            PlayerResults p;
            p.username = pair.first;
            p.correctAnswerCount = pair.second.correctAnswerCount;
            p.wrongAnswerCount = pair.second.wrongAnswerCount;
            p.averageAnswerTime = pair.second.averageAnswerTime;
            res.results.push_back(p);
        }
    }

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeGetGameResultsResponse(res);
    result.newHandler = this;

    return result;
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo& requestInfo)
{
    m_game.removePlayer(m_user.username);

    if (m_game.isFinished())
    {
        m_handlerFactory.getRoomManager()->deleteRoom(m_game.getGameId());
    }

    LeaveGameResponse res;
    res.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeLeaveGameResponse(res);
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.username);

    return result;
}