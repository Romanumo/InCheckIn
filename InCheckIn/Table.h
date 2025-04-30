#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "AnimationManager.h"
using namespace Engine;

enum GameFlow
{
    CHOOSING,
    PLAYER, 
    ENEMY
};

class Hand;
class Field;
class Deck;

class Table : public Engine::GameObject
{
public:
    Table();
    void NextTurn();

    void ChangeSpiralCombo(int spiralChange);
    void ApplySpiralCombo();

    void SetHammerMode(bool hammerMode);
    int GetSpiral();
    bool GetHammerMode();

private:
    Field* enemyField = nullptr;
    Field* playerField = nullptr;
    Hand* hand = nullptr;
    
    std::unique_ptr<Deck> deck;
    bool isHammer = false;
    Image* hammerImage;

    GameFlow turn = GameFlow::CHOOSING;
    Text* spiralText;
    Text* spiralComboText;

    int spiral = 10;
    int spiralCombo = 0;
    int neededSpiral = 30;

    void Lose();
    void Win();

    void CreateTable();
    std::unique_ptr<GameObject> CreateTurnBell();
    std::unique_ptr<GameObject> CreateHammer();
    std::unique_ptr<GameObject> CreateSpiralResource();
};