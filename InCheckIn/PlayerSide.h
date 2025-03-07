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

        auto cardGrid = std::make_unique<GameObject>(0, 0, 0, 0);
        Layout* cardRow = new Layout(cardGrid.get(), new Row(), Config::PADDING, 0);
        for (int i = 0;i < Config::SIDE_MAX_CARDS;i++)
        {
            auto cell = std::make_unique<Cell>(Config::CARD_WIDTH, Config::CARD_HEIGHT);
            cells.push_back(cell.get());
            cardRow->AddGameObject(std::move(cell));
        }
        cardGrid->AddComponent(cardRow);

        auto handOriginal = std::make_unique<CardHand>(0, 0);
        hand = handOriginal.get();
        ConnectCellsToHand(hand);

        auto participant = std::make_unique<Participant>
            (Config::CARD_WIDTH, Config::CARD_HEIGHT * 2, Config::DESK_IMAGE);
        player = participant.get();

        auto playableUI = std::make_unique<GameObject>(0, 0, 0, 0);
        Layout* playUICol = new Layout(playableUI.get(), new Column(), Config::PADDING, 0);
        playUICol->AddGameObject(std::move(cardGrid));
        playUICol->AddGameObject(std::move(handOriginal));
        playableUI->AddComponent(playUICol);

        Layout* row = new Layout(this, new Row(), Config::PADDING, 0);
        row->AddGameObject(std::move(playableUI));
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