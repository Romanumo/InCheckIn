#pragma once
#include <cassert>
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "PopUpManager.h"
using namespace Engine;

enum GameFlow
{
    CHOOSING,
    PLAYER, 
    ENEMY
};

//Add Unsubscribe
class GameManager
{
public:
    GameManager()
    {
        if (isInstantiated) throw std::runtime_error("GameManager already exists");
        isInstantiated = true;

        onNewGame.push_back([this](int level) {
            goal += 5;
            levelsCompleted++;
            });
    }

    ~GameManager() { isInstantiated = false; }

    void NextTurn()
    {
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

    void AddOnWin(std::function<void()> event) { onWin = event; }

    int GetReq() { return goal; }
    int GetLevel() { return levelsCompleted; }
    GameFlow GetState() { return turn; }
private:
    inline static bool isInstantiated = false;
    GameFlow turn = GameFlow::CHOOSING;

    int levelsCompleted = -1;
    int goal = 10;
    int* measure = nullptr;

    std::vector<std::function<void(GameFlow)>> onTurnChange;
    std::vector<std::function<void(int)>> onNewGame;
    std::function<void()> onWin;

    void CheckGoal()
    {
        if (*measure < 0) std::cout << "Loser!" << std::endl;
        if (*measure > goal)
        {
            if(onWin) onWin();
            NewGame();
        }
    }
};