#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
using namespace Engine;

enum CardState
{
	IDLE,
	HOVERED,
	CHOSEN,
	PLAYED
};

class Card : public GameObject
{
public:
	Card(CardStats stats) : GameObject(0,0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT), 
		stats(stats)
	{
		button = new Button(this);
		Image* image = new Image(this, stats.imagePath);

		AssignButton(button);

		AdoptChild(std::move(UIFactory::NewText(
			10, Conf::CARD_HEIGHT - 30, Conf::CARD_WIDTH / 5, 20,
			sanityText)));
		sanityText->SetText(std::to_string(stats.sanity), Conf::SANITY_COLOR);

		AddComponent(image);
		AddComponent(button);
	}

	void IntoPlayedState()
	{
		button->SetEnabled(false);
		state = CardState::PLAYED;
	}

	void Deselect()
	{
		SetRelPosition(GetRelTf()->x, initY);
		state = CardState::IDLE;
	}

	CardState GetState() { return state; }
	std::string GetName() { return stats.name; }

private:
	Button* button;
	Text* sanityText;

	CardState state = CardState::IDLE;
	int initY = 0;

	CardStats stats;

	void AssignButton(Engine::Button* button)
	{
		this->initY = GetRelTf()->y;
		button->AddOnHoverEnter([this] {
			if (state == CardState::IDLE)
			{
				SetRelPosition(GetRelTf()->x, GetRelTf()->y - 5);
				state = CardState::HOVERED;
			}
			});

		button->AddOnLeftClick([this] {
			if (state == CardState::HOVERED)
			{
				SetRelPosition(GetRelTf()->x, GetRelTf()->y - 15);
				state = CardState::CHOSEN;
			}
			});

		button->AddOnHoverExit([this] {
			if (state == CardState::HOVERED)
			{
				Deselect();
			}
			});
	}
};