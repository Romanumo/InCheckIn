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
        cells = UIFactory::GetRow<Cell>(cardGrid.get(), Config::SIDE_MAX_CARDS,
            Config::CARD_WIDTH, Config::CARD_HEIGHT);

        auto handOriginal = std::make_unique<CardHand>(0, 0);
        hand = handOriginal.get();
        ConnectCellsToHand(hand);

        auto participant = std::make_unique<Participant>
            (Config::CARD_WIDTH, Config::CARD_HEIGHT * 2, Config::DESK_IMAGE);
        player = participant.get();

        auto playableUI = std::make_unique<GameObject>(0, 0, 0, 0);
        playableUI->AddComponent(new Column(playableUI.get(), Config::PADDING, 0, 
            std::move(cardGrid),std::move(handOriginal)));

        AddComponent(new Row(this, 0, 0, 
            std::vector<std::unique_ptr<GameObject>>{
            std::move(playableUI),
            std::move(participant) }));
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