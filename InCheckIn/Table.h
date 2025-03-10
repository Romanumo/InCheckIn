#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "PlayerSide.h"
#include "EnemySide.h"

class Table : public Engine::GameObject
{
public:
    Table() : Engine::GameObject(Conf::PADDING, Conf::PADDING, 0, 0)
    {
        auto enemyOriginal = std::make_unique<EnemySide>();
        enemySide = enemyOriginal.get();

        auto playerOriginal = std::make_unique<PlayerSide>();
        playerSide = playerOriginal.get();

        Engine::Layout* col = new Engine::Layout(this, new Engine::Column(),
            Conf::PADDING, 0);
        col->AddGameObject(std::move(enemyOriginal));
        col->AddGameObject(std::move(playerOriginal));
        AddComponent(col);
    }

private:
    EnemySide* enemySide = nullptr;
    PlayerSide* playerSide = nullptr;
};