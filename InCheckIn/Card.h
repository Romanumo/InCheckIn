#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"

class Card : public Engine::GameObject
{
public:

	Card(int x, int y) : 
		GameObject(x, y, Config::CARD_WIDTH, Config::CARD_HEIGHT)
	{
		using namespace Engine;
		button = new Button(this);
		Image* image = new Image(this, Config::CARD_IMAGE_HEALER);
		button->AddOnHoverEnter([this] {
			SetRelSize(Config::CARD_WIDTH + 10, Config::CARD_HEIGHT + 10);
			});

		button->AddOnHoverExit([this] {
			SetRelSize(Config::CARD_WIDTH, Config::CARD_HEIGHT);
			});

		this->AddComponent(image);
		this->AddComponent(button);
	}

private:
	Engine::Button* button;
};