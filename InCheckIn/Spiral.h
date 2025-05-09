#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
using namespace Engine;

class Spiral : public GameObject
{
public:
	Spiral(GameManager& GM) : 
        GameObject(0, 0, Conf::AVATAR_WIDTH, Conf::AVATAR_HEIGHT), GM(&GM)
	{
        int w = Conf::AVATAR_WIDTH;
        int h = Conf::AVATAR_HEIGHT;

        AddComponent(new Image(this, Conf::SPIRAL_IMAGE));

        AdoptChild(std::move(UIFactory::NewText(w / 2, h / 2 - 20, w / 2, 40, spiralText)));
        spiralText->SetText(std::to_string(spiral) + "/" + std::to_string(GM.GetReq()),
            Conf::SPIRAL_COLOR);

        AdoptChild(std::move(UIFactory::NewText(w / 2, h / 2 + 10, w / 2, 40, spiralComboText)));
        spiralComboText->SetText("0", Conf::SPIRAL_COLOR);

        GM.AddOnTurnChange([this](GameFlow turn) {
           if(turn == GameFlow::CHOOSING) ApplySpiralCombo();
        });

        GM.AddOnNewGame([this]() {
            spiralCombo = 0;
            spiral = 10;
            ApplySpiralCombo();
        });
	}

    void ChangeSpiralCombo(int spiralChange)
    {
        spiralCombo += spiralChange;
        std::string sign = (spiralCombo > 0) ? "+" : "";
        spiralComboText->SetText(sign + std::to_string(spiralCombo));
    }

    void ApplySpiralCombo()
    {
        spiral += spiralCombo;
        spiralText->SetText(std::to_string(spiral) + "/" + std::to_string(GM->GetReq()));

        spiralCombo = 0;
        spiralComboText->SetText("0");
    }

    int GetSpiral() { return spiral; }
    int* GetSpiralRef() { return &spiral; }
private:
    Text* spiralText;
    Text* spiralComboText;

    int spiral = 10;
    int spiralCombo = 0;

    GameManager* GM;
};