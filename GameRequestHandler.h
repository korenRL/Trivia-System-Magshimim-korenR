#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "GameManager.h"
#include "Game.h"
#include <string>

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
private:
    Game& m_game;
    LoggedUser m_user;
    GameManager& m_gameManager;
    RequestHandlerFactory& m_handlerFactory;

public:
    GameRequestHandler(
        Game& game,
        LoggedUser user,
        GameManager& gameManager,
        RequestHandlerFactory& handlerFactory
    );

    ~GameRequestHandler();

    virtual bool isRequestRelevant(
        const RequestInfo& requestInfo
    ) const override;

    virtual RequestResult handleRequest(
        const RequestInfo& requestInfo
    ) override;

private:
    RequestResult getQuestion(const RequestInfo& requestInfo);
    RequestResult submitAnswer(const RequestInfo& requestInfo);
    RequestResult getGameResults(const RequestInfo& requestInfo);
    RequestResult leaveGame(const RequestInfo& requestInfo);
};