#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"

class Participant : public Engine::GameObject
{
public:
    Participant(const std::string& imagePath, 
        const std::string& nameString) :
        Engine::GameObject(0, 0, Conf::PARTICIPANT_WIDTH, Conf::PARTICIPANT_HEIGHT)
    {
        using namespace Engine;
        int w = Conf::PARTICIPANT_WIDTH;
        int h = Conf::PARTICIPANT_HEIGHT;

        Image* image = new Image(this, imagePath);
        AddComponent(image);

        AdoptChild(std::move(UIFactory::NewText(0, 10, w, 30, nameText)));
        nameText->SetText(nameString);

        AdoptChild(std::move(UIFactory::NewText(10, h - 30, w / 5, 20, sanityText)));
        sanityText->SetText(std::to_string(sanity), Conf::SANITY_COLOR);
    }

private:
    Engine::Text* sanityText;
    Engine::Text* nameText;
    int sanity = 10;
};