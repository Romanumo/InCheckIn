#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"

class TurnButton : public Engine::GameObject
{
public:
    TurnButton() :
        Engine::GameObject(0, 0, Conf::TURNBT_WIDTH, Conf::TURNBT_HEIGHT)
    {
        using namespace Engine;
        Image* image = new Image(this, Conf::TURNBT_IMAGE);
        AddComponent(image);

        Text* text;
        AdoptChild(std::move(UIFactory::NewText(0, 0, 
            GetAbsTf()->w, GetAbsTf()->h, text)));
        text->SetText("End Turn");

        button = new Button(this);
        AddComponent(button);
    }

private:
    Engine::Button* button;
};