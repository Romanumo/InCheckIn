#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"

class Participant : public Engine::GameObject
{
public:
    Participant(int w, int h, const std::string& imagePath) : 
        Engine::GameObject(0,0, w, h)
    {
        using namespace Engine;
        Image* image = new Image(this, imagePath);

        AddComponent(image);
    }
};