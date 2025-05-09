#pragma once
#include <cassert>
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "GameUIBuilder.h"
#include "HintManager.h"
using namespace Engine;

class SceneManager
{
public:
    SceneManager()
    {
        if (isInstantiated) throw std::runtime_error("SceneManager already exists");
        isInstantiated = true;

        CreateScenes();
    }

    ~SceneManager() { isInstantiated = false; }

    void Render(SDL_Surface* surface)
    {
        mainScene->Render(surface);
        globalUI->Render(surface);
    }

    void HandleInput(const SDL_Event& event)
    {
        mainScene->HandleEvent(event);
        globalUI->HandleEvent(event);
    }

private:
    inline static bool isInstantiated = false;

    GameObject* mainScene;
    std::unique_ptr<GameObject> globalUI;
    std::unique_ptr<GameObject> gameScene;
    std::unique_ptr<GameObject> shopScene;

    void CreateScenes()
    {
        gameScene = std::make_unique<GameObject>(Conf::PADDING, Conf::PADDING, 0, 0);
        shopScene = std::make_unique<GameObject>(Conf::WINDOW_WIDTH / 3, Conf::WINDOW_HEIGHT / 3, 0, 0);
        globalUI = std::make_unique<GameObject>(0, 0, 0, 0);
        mainScene = gameScene.get();

        GameUIBuilder::CreateTable(gameScene.get(), [this] {GoToShop();});
        GameUIBuilder::CreateShop(shopScene.get(), [this] {GoToGame();});

        HintManager::GetInstance().Init(globalUI.get());
    }

    void GoToShop()
    {
        mainScene = shopScene.get();
    }

    void GoToGame()
    {
        mainScene = gameScene.get();
    }
};
