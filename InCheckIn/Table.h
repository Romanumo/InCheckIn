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
        std::vector<std::unique_ptr<Engine::GameObject>> parts;
        for (int i = 0;i < 2;i++)
        {
            auto side = std::make_unique<TableSide>();
            if (!enemySide) enemySide = side.get();
            else if (!playerSide) playerSide = side.get();

            parts.push_back(std::move(side));
        }

        auto handOriginal = std::make_unique<CardHand>(0,0);
        hand = handOriginal.get();
        parts.push_back(std::move(handOriginal));

        playerSide->AssignHand(hand);
        Engine::UIFactory::GetColumnComponent(this, std::move(parts));
    }

private:
    TableSide* enemySide = nullptr;
    TableSide* playerSide = nullptr;
    CardHand* hand;
};