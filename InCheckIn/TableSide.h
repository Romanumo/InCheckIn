#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "Cell.h"
#include "CardHand.h"

class TableSide : public Engine::GameObject
{
public:
    TableSide() : Engine::GameObject(0, 0, 0, 0)
    {
        using namespace Engine;

        std::vector<std::unique_ptr<GameObject>> cardsPlaceholder;
        for (int i = 0;i < Config::SIDE_MAX_CARDS;i++)
        {
            auto cell = std::make_unique<Cell>(0,0, Config::CARD_WIDTH, Config::CARD_HEIGHT);

            cells.push_back(cell.get());
            cardsPlaceholder.push_back(std::move(cell));
        }

        UIFactory::GetRowComponent(this, std::move(cardsPlaceholder));
    }

    void AssignToPlayer(CardHand* hand)
    {
        for (Cell* cell : cells)
        {
            cell->AssignToPlayer(hand);
        }
    }

private:
    std::vector<Cell*> cells;
};