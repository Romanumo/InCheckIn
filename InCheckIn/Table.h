#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "Field.h"
#include "Hand.h"

class Table : public Engine::GameObject
{
public:
    Table() : Engine::GameObject(Conf::PADDING, Conf::PADDING, 0, 0)
    {
        auto eFieldObj = std::make_unique<Field>("Concussion", Conf::DESK_IMAGE);
        enemyField = eFieldObj.get();

        auto handObj = std::make_unique<Hand>();

        auto pFieldObj = std::make_unique<Field>("You", Conf::DESK_IMAGE, handObj.get());
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

        playerField->SetOpposingField(enemyField);
    }

private:
    Field* enemyField = nullptr;
    Field* playerField = nullptr;

    std::unique_ptr<GameObject> CreateTurnBell()
    {
        auto bellObj = std::make_unique<GameObject>(0, 0, 
            Conf::TURNBT_WIDTH, Conf::TURNBT_HEIGHT);
        Image* image = new Image(bellObj.get(), Conf::TURNBT_IMAGE);
        bellObj->AddComponent(image);

        bellObj->AdoptChild(std::move(UIFactory::NewText(0, 0,
            Conf::TURNBT_WIDTH, Conf::TURNBT_HEIGHT, "End Turn")));

        Button* button = new Button(bellObj.get());

        button->AddOnLeftClick([this] {
            NextTurn();
            });

        bellObj->AddComponent(button);
        return bellObj;
    }

    void NextTurn()
    {

    }
};