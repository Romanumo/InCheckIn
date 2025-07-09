#pragma once
#include <cassert>
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "AnimationManager.h"
#include "PopUpManager.h"
using namespace Engine;

enum GameFlow
{
    CHOOSING,
    PLAYER, 
    ENEMY
};

class GameManager
{
public:
    GameManager()
    {
        if (isInstantiated) throw std::runtime_error("GameManager already exists");
        isInstantiated = true;

        onNewGame.push_back([this](int level) {
            isActive = true;
            goal += 5;
            levelsCompleted++;
            });
    }

    ~GameManager() { isInstantiated = false; }

    void NextTurn()
    {
        if (!isActive) return;

        switch (turn)
        {
        case CHOOSING:
            turn = GameFlow::PLAYER;
            break;
        case PLAYER:
            turn = GameFlow::ENEMY;
            break;
        case ENEMY:
            turn = GameFlow::CHOOSING;
            break;
        }

        for(auto event : onTurnChange) event(turn);
        if (turn == GameFlow::CHOOSING) CheckGoal();
    }

    void SetMeasureRef(int* measure) { this->measure = measure; }
    void NewGame() { for (auto event : onNewGame) event(levelsCompleted); }

    void AddOnTurnChange(std::function<void(GameFlow)> event) { onTurnChange.push_back(event); }

    void AddOnNewGame(std::function<void()> event) { onNewGame.push_back([event](int level) {event();}); }
    void AddOnNewGame(std::function<void(int)> event) { onNewGame.push_back(event); }

    void AddOnWin(std::function<void()> event) { onWinNextScene = event; }

    int GetReq() { return goal; }
    int GetLevel() { return levelsCompleted; }
    GameFlow GetState() { return turn; }
private:
    inline static bool isInstantiated = false;
    GameFlow turn = GameFlow::CHOOSING;
    bool isActive = true;

    int levelsCompleted = -1;
    int goal = 10;
    int* measure = nullptr;

    std::vector<std::function<void(GameFlow)>> onTurnChange;
    std::vector<std::function<void(int)>> onNewGame;
    std::function<void()> onWinNextScene;

    void CheckGoal()
    {
        if (*measure < 0)
        {
            isActive = false;
            PopUpManager::GetInstance().CallWindow("You Lost!");
            AnimationManager::GetInstance().PlayDelayedAnimation([this] {
                SDL_Event quitEvent{ SDL_QUIT };
                SDL_PushEvent(&quitEvent);
            }, 2000);
        }
        else if (*measure > goal)
        {
            isActive = false;
            PopUpManager::GetInstance().CallWindow("You Won!");

            AnimationManager::GetInstance().PlayDelayedAnimation([this] {
                PopUpManager::GetInstance().HideWindow();
                if (onWinNextScene) onWinNextScene();
                NewGame();
            }, 2000);
        }
    }
};