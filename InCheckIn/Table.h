#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "AnimationManager.h"
#include "Field.h"
#include "Hand.h"

enum GameFlow
{
    CHOOSING,
    PLAYER, 
    ENEMY
};

class Table : public Engine::GameObject
{
public:
    Table() : Engine::GameObject(Conf::PADDING, Conf::PADDING, 0, 0)
    {
        CreateTable();
        playerField->SetOpposingField(enemyField);

        enemyField->PlaceCard(CardFactory::Basic(), 2);
    }

    void NextTurn()
    {
        switch (turn)
        {
        case CHOOSING:
            std::cout << "Chosing Ends" << std::endl;
            turn = GameFlow::PLAYER;
            playerField->SetEnabled(false);
            playerField->PlayTurn();
            break;
        case PLAYER:
            std::cout << "Player Turn Ends" << std::endl;
            turn = GameFlow::ENEMY;
            enemyField->PlayTurn();
            break;
        case ENEMY:
            std::cout << "Enemy Turn Ends" << std::endl;
            turn = GameFlow::CHOOSING;
            playerField->SetEnabled(true);
            break;
        }
    }

private:
    Field* enemyField = nullptr;
    Field* playerField = nullptr;
    GameFlow turn = GameFlow::CHOOSING;

    #pragma region Init
    void CreateTable()
    {
        auto eFieldObj = std::make_unique<Field>("Hobby", Conf::DESK_IMAGE, this);
        enemyField = eFieldObj.get();

        auto handObj = std::make_unique<Hand>();

        auto pFieldObj = std::make_unique<Field>("You", Conf::DESK_IMAGE, this, 
            handObj.get(), 10);
        playerField = pFieldObj.get();

        Engine::Layout* col = new Engine::Layout(this, new Engine::Column(),
            Conf::PADDING, 0);
        col->AddGameObject(std::move(eFieldObj));
        col->AddGameObject(std::move(pFieldObj));
        col->AddGameObject(std::move(
            UIFactory::NewRow(
                std::move(handObj),
                std::move(CreateTurnBell()))));
        AddComponent(col);
    }

    std::unique_ptr<GameObject> CreateTurnBell()
    {
        auto bellObj = std::make_unique<GameObject>(0, 0,
            Conf::TURNBT_WIDTH, Conf::TURNBT_HEIGHT);
        Image* image = new Image(bellObj.get(), Conf::TURNBT_IMAGE);

        bellObj->AdoptChild(std::move(UIFactory::NewText(0, 0,
            Conf::TURNBT_WIDTH, Conf::TURNBT_HEIGHT, "End Turn")));

        Button* button = new Button(bellObj.get());

        button->AddOnLeftClick([this] {
            if(turn == GameFlow::CHOOSING) NextTurn();
            });

        bellObj->AddComponent(image);
        bellObj->AddComponent(button);
        return bellObj;
    }
    #pragma endregion
};