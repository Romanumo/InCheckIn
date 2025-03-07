#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"

#include "Cell.h"
#include "CardHand.h"
#include "Participant.h"

class EnemySide : public Engine::GameObject
{
public:
    EnemySide() : Engine::GameObject(0, 0, 0, 0)
    {
        using namespace Engine;

        Layout* row = new Layout(this, new Row(), Config::PADDING, 0);
        for (int i = 0;i < Config::SIDE_MAX_CARDS;i++)
        {
            auto cell = std::make_unique<Cell>(Config::CARD_WIDTH, Config::CARD_HEIGHT);
            cells.push_back(cell.get());
            row->AddGameObject(std::move(cell));
        }

        auto participant = std::make_unique<Participant>
            (Config::CARD_WIDTH, Config::CARD_HEIGHT, Config::DESK_IMAGE);
        enemy = participant.get();
        row->AddGameObject(std::move(participant));

        AddComponent(row);
    }
private:
    std::vector<Cell*> cells;
    Participant* enemy;
};