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

        cells = UIFactory::GetRow<Cell>(this, Config::SIDE_MAX_CARDS,
            Config::CARD_WIDTH, Config::CARD_HEIGHT);

        auto participant = std::make_unique<Participant>
            (Config::CARD_WIDTH, Config::CARD_HEIGHT, Config::DESK_IMAGE);
        enemy = participant.get();
        GetComponent<Row>()->AddGameObject(std::move(participant));
    }
private:
    std::vector<Cell*> cells;
    Participant* enemy;
};