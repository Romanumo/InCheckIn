#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "PlayerSide.h"
#include "EnemySide.h"

class Table : public Engine::GameObject
{
public:
    Table() : Engine::GameObject(Config::PADDING, Config::PADDING, 0, 0)
    {
        auto enemyOriginal = std::make_unique<EnemySide>();
        enemySide = enemyOriginal.get();

        auto playerOriginal = std::make_unique<PlayerSide>();
        playerSide = playerOriginal.get();

        AddComponent(new Engine::Column(this, Config::PADDING, 0,
            std::move(enemyOriginal), std::move(playerOriginal)));
    }

private:
    EnemySide* enemySide = nullptr;
    PlayerSide* playerSide = nullptr;
};