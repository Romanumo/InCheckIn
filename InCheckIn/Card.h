#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"

//Needs state
class Card : public Engine::GameObject
{
public:
	Card(int x, int y) : 
		GameObject(x, y, Config::CARD_WIDTH, Config::CARD_HEIGHT)
	{
		using namespace Engine;
		button = new Button(this);
		Image* image = new Image(this, Config::CARD_IMAGE_HEALER);
		button->AddOnHoverEnter([this, y] {
			this->y = GetRelTf()->y;
			SetRelPosition(GetRelTf()->x, GetRelTf()->y - 5);
			});

		button->AddOnLeftClick([this, y] {
			SetRelPosition(GetRelTf()->x, GetRelTf()->y - 15);
			});

		button->AddOnHoverExit([this, y] {
			SetRelPosition(GetRelTf()->x, y);
			});

		this->AddComponent(image);
		this->AddComponent(button);
	}

private:
	Engine::Button* button;
	int y = 0;
};