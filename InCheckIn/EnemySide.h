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

        cells = UIFactory::GetLayout<Cell>(this, new Row(),Conf::MAX_CARDS);

        auto participant = std::make_unique<Participant>(Conf::DESK_IMAGE, "Conjurer");
        enemy = participant.get();

        GetComponent<Layout>()->AddGameObject(std::move(participant));
    }
private:
    std::vector<Cell*> cells;
    Participant* enemy;
};