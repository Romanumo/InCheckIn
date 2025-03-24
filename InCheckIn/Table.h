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

    bool SpendSpiral(int spiralCost)
    {
        if (spiralCost > spiral) return false;
        spiral -= spiralCost;
        spiralText->SetText(std::to_string(spiral));
        return true;
    }

    void AddSpiral(int spiralAdd)
    {
        spiral += spiralAdd;
        spiralText->SetText(std::to_string(spiral));
    }

private:
    Field* enemyField = nullptr;
    Field* playerField = nullptr;
    GameFlow turn = GameFlow::CHOOSING;

    Text* spiralText;
    int spiral = 10;

    #pragma region Init
    void CreateTable()
    {
        auto eFieldObj = std::make_unique<Field>(this);
        enemyField = eFieldObj.get();

        auto handObj = std::make_unique<Hand>();

        auto pFieldObj = std::make_unique<Field>(this, handObj.get());
        playerField = pFieldObj.get();

        Engine::Layout* col = new Engine::Layout(this, new Engine::Column(),
            Conf::PADDING, 0);
        col->AddGameObject(std::move(
            UIFactory::NewRow(
                std::move(eFieldObj),
                std::move(CreateAvatar("Hobby", Conf::DESK_IMAGE)))));
        col->AddGameObject(std::move(
            UIFactory::NewRow(
                std::move(pFieldObj),
                std::move(CreateSpiralResource()))));
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

    std::unique_ptr<GameObject> CreateAvatar(const std::string& name, 
        const std::string& imagePath)
    {
        int w = Conf::AVATAR_WIDTH;
        int h = Conf::AVATAR_HEIGHT;

        auto avatar = std::make_unique<GameObject>(0, 0, w, h);
        avatar->AddComponent(new Image(avatar.get(), imagePath));
        avatar->AdoptChild(std::move(UIFactory::NewText(0, 10, w, 30, name)));
        return avatar;
    }

    std::unique_ptr<GameObject> CreateSpiralResource()
    {
        int w = Conf::AVATAR_WIDTH;
        int h = Conf::AVATAR_HEIGHT;

        auto spiralIcon = std::make_unique<GameObject>(0, 0, w, h);
        spiralIcon->AddComponent(new Image(spiralIcon.get(), Conf::SPIRAL_IMAGE));
        spiralIcon->AdoptChild(std::move(UIFactory::NewText(w/3, h / 2, w / 3, 20, spiralText)));
        spiralText->SetText(std::to_string(spiral), Conf::SPIRAL_COLOR);
        return spiralIcon;
    }
    #pragma endregion
};