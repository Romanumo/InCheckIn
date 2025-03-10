#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"

class Participant : public Engine::GameObject
{
public:
    Participant(int w, int h, const std::string& imagePath, 
        const std::string& nameString) :
        Engine::GameObject(0, 0, w, h)
    {
        using namespace Engine;
        Image* image = new Image(this, imagePath);
        AddComponent(image);

        AdoptChild(std::move(UIFactory::NewText(0, 10, w, 30, 
            nameText)));
        nameText->SetText(nameString);

        AdoptChild(std::move(UIFactory::NewText(10, h - 30, w / 5, 20,
            sanityText)));
        sanityText->SetText("0", Conf::SANITY_COLOR);
    }

private:
    Engine::Text* sanityText;
    Engine::Text* nameText;
    int sanity = 0;
};