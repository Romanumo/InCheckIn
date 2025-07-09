#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "GameManager.h"
using namespace Engine;

class Hammer : public GameObject
{
public:
	Hammer(GameManager& GM) : GameObject(0, 0, Conf::AVATAR_WIDTH, Conf::AVATAR_HEIGHT)
	{
        image = new Image(this, Conf::HAMMER_OFF_IMAGE);
        AddComponent(image);

        Button* button = new Button(this);
        button->AddOnLeftClick([&] {
            SetHammerMode(!isHammer);
            });

        AddComponent(button);

        GM.AddOnTurnChange([this](GameFlow turn) {
            if (turn == GameFlow::PLAYER) SetHammerMode(false);
        });
	}

    void SetHammerMode(bool hammerMode)
    {
        isHammer = hammerMode;

        if (isHammer) 
        {
            image->SetImage(Conf::HAMMER_ON_IMAGE);
            SoundManager::GetInstance().PlaySFX(Conf::SOUND_HAMMER_ON);
        }
        else image->SetImage(Conf::HAMMER_OFF_IMAGE);
    }

    bool GetHammerMode() { return isHammer; }

private:
    bool isHammer = false;
    Image* image;
};