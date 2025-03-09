#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"

#include "Cell.h"
#include "CardHand.h"
#include "Participant.h"

class PlayerSide : public Engine::GameObject
{
public:
    PlayerSide() : Engine::GameObject(0, 0, 0, 0)
    {
        using namespace Engine;

        auto cellGrid = std::make_unique<GameObject>();
        cells = UIFactory::GetLayout<Cell>(cellGrid.get(), new Row(), 
            Config::SIDE_MAX_CARDS);

        auto handOriginal = std::make_unique<CardHand>();
        hand = handOriginal.get();
        ConnectCellsToHand(hand);

        auto participant = std::make_unique<Participant>
            (Config::CARD_WIDTH, Config::CARD_HEIGHT, Config::DESK_IMAGE);
        player = participant.get();

        Layout* row = new Layout(this, new Row(), Config::PADDING, 0);
        row->AddGameObject(std::move(UIFactory::GetColumn(
            std::move(cellGrid), 
            std::move(handOriginal))));
        row->AddGameObject(std::move(participant));
        AddComponent(row);
    }

    void ConnectCellsToHand(CardHand* hand)
    {
        for (Cell* cell : cells)
        {
            cell->AssignToPlayer(hand);
        }
    }

private:
    std::vector<Cell*> cells;
    Participant* player;
    CardHand* hand;
};