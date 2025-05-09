#pragma once
#include <cassert>
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
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
    void NewGame() { for (auto event : onNewGame) event(); }

    void AddOnTurnChange(std::function<void(GameFlow)> event) { onTurnChange.push_back(event); }
    void AddOnNewGame(std::function<void()> event) { onNewGame.push_back(event); }

    void AddOnWin(std::function<void()> event) { onWin = event; }

    int GetReq() { return goal; }
    GameFlow GetState() { return turn; }
private:
    inline static bool isInstantiated = false;
    GameFlow turn = GameFlow::CHOOSING;

    int goal = 15;
    int* measure = nullptr;

    std::vector<std::function<void(GameFlow)>> onTurnChange;
    std::vector<std::function<void()>> onNewGame;
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