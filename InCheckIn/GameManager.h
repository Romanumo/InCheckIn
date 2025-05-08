#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "AnimationManager.h"
#include "EnemyAI.h"
#include "Shop.h"
#include "Deck.h"
#include "Field.h"
#include "Hand.h"

enum GameFlow
{
    CHOOSING,
    PLAYER, 
    ENEMY
};

class Hand;
class Field;
class Deck;

class GameManager
{
public:
    static void Init();
    static void NextTurn();
    static GameObject* GetScene();
    static void SwitchToGame();

    static void ChangeSpiralCombo(int spiralChange);
    static int GetSpiral();
    static void ApplySpiralCombo();

    static void SetHammerMode(bool hammerMode);
    static bool GetHammerMode();

    static void Render(SDL_Surface* surface);
    static void HandleInput(const SDL_Event& event);

    static Deck* GetDeck();

    GameManager() = delete;
private:
    //Game Flow
    static GameObject* mainScene;
    static std::unique_ptr<GameObject> globalUI;
    static std::unique_ptr<GameObject> gameScene;
    static std::unique_ptr<GameObject> shopScene;

    static Field* enemyField;
    static Field* playerField;
    static Shop* shop;
    static Hand* hand;

    static GameFlow turn;
    static EnemyAI enemyAI;
    static Deck deck;
    
    //Hammer
    static bool isHammer;
    static Image* hammerImage;

    //Spiral
    static Text* spiralText;
    static Text* spiralComboText;
    static int spiral;
    static int spiralCombo;
    static int neededSpiral;

    static void Lose();
    static void Win();

    static void CreateTable();
    static std::unique_ptr<GameObject> CreateTurnBell();
    static std::unique_ptr<GameObject> CreateHammer();
    static std::unique_ptr<GameObject> CreateSpiralResource();

    static void CreateShop();
};