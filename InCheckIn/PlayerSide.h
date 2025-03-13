#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"

#include "Cell.h"
#include "CardHand.h"
#include "Participant.h"
#include "TurnButton.h"

class PlayerSide : public Engine::GameObject
{
public:
    PlayerSide() : Engine::GameObject(0, 0, 0, 0)
    {
        using namespace Engine;

        auto cellGrid = std::make_unique<GameObject>();
        cells = UIFactory::GetLayout<Cell>(cellGrid.get(), new Row(), 
            Conf::MAX_CARDS);

        auto handOriginal = std::make_unique<CardHand>();
        hand = handOriginal.get();
        ConnectCellsToHand(hand);

        auto participant = std::make_unique<Participant>(Conf::DESK_IMAGE, "You");
        player = participant.get();

        auto turnButton = std::make_unique<TurnButton>();

        Layout* row = new Layout(this, new Row(), Conf::PADDING, 0);
        row->AddGameObject(std::move(UIFactory::NewColumn(
            std::move(cellGrid), 
            std::move(handOriginal))));

        row->AddGameObject(std::move(UIFactory::NewColumn(
            std::move(participant),
            std::move(turnButton))));

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