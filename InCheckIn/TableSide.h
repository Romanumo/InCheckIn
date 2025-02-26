#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "CardHand.h"

class TableSide : public Engine::GameObject
{
public:
    TableSide() : Engine::GameObject(0, 0, 0, 0)
    {
        using namespace Engine;
        //Image* sideImage = new Image(this, Config::DESK_IMAGE);
        //this->AddComponent(sideImage);

        std::vector<GameObject*> cardsPlaceholder;
        for (int i = 0;i < Config::SIDE_MAX_CARDS;i++)
        {
            GameObject* placeholder = new GameObject(0, 0,
                Config::CARD_WIDTH, Config::CARD_HEIGHT);
            Image* image = new Image(placeholder, Config::PLACEHOLDER_IMAGE);
            placeholder->AddComponent(image);
            cardsPlaceholder.push_back(placeholder);
        }

        UIFactory::GetRowComponent(this, cardsPlaceholder);
    }
};