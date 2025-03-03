#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"

enum CardState
{
	IDLE,
	HOVERED,
	CHOSEN,
	PLAYED
};

class Card : public Engine::GameObject
{
public:
	Card(int x, int y) : 
		GameObject(x, y, Config::CARD_WIDTH, Config::CARD_HEIGHT)
	{
		using namespace Engine;
		button = new Button(this);
		Image* image = new Image(this, Config::CARD_IMAGE_HEALER);

		this->y = GetRelTf()->y;
		button->AddOnHoverEnter([this, y] {
			if (state == CardState::IDLE)
			{
				SetRelPosition(GetRelTf()->x, GetRelTf()->y - 5);
				state = CardState::HOVERED;
			}
			});

		button->AddOnLeftClick([this, y] {
			if (state == CardState::HOVERED)
			{
				SetRelPosition(GetRelTf()->x, GetRelTf()->y - 15);
				state = CardState::CHOSEN;
			}
			});

		button->AddOnHoverExit([this, y] {
			if (state == CardState::HOVERED)
			{
				Deselect();
			}
			});

		this->AddComponent(image);
		this->AddComponent(button);
	}

	void IntoPlayedState()
	{
		state = CardState::PLAYED;
	}

	void Deselect()
	{
		SetRelPosition(GetRelTf()->x, y);
		state = CardState::IDLE;
	}

	CardState GetState() { return state; }

private:
	Engine::Button* button;

	CardState state = CardState::IDLE;
	int y = 0;
};