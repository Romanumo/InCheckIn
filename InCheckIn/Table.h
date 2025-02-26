#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "CardHand.h"
#include "TableSide.h"

class Table : public Engine::GameObject
{
public:
    Table() : Engine::GameObject(Config::PADDING, Config::PADDING, 0, 0)
    {
        std::vector<Engine::GameObject*> parts;
        for (int i = 0;i < 2;i++)
        {
            TableSide* side = new TableSide();
            if (!enemySide) enemySide = side;
            else if (!playerSide) playerSide = side;

            parts.push_back(side);
        }
        hand = new CardHand(0, 0);
        parts.push_back(hand);

        Engine::UIFactory::GetColumnComponent(this, parts);
    }

private:
    TableSide* enemySide = nullptr;
    TableSide* playerSide = nullptr;
    CardHand* hand;
};