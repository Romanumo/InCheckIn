#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "Minion.h"
using namespace Engine;

enum CardState
{
	IDLE,
	HOVERED,
	CHOSEN
};

class Card : public Button
{
public:
	Card(GameObject* parent, Minion* minion) : Button(parent), minion(minion)
	{
		AssignButton();
	}

	Minion* TransformToMinion(Field* field)
	{
		minion->SetParent(GetParent());
		GetParent()->AddComponent(minion);

		return minion;
	}

	void Deselect()
	{
		GetParent()->SetRelPosition(GetParent()->GetRelTf()->x, initY);
		state = CardState::IDLE;
	}

	std::string GetName() { return minion->GetName(); }
	CardState GetState() { return state; }

private:
	Minion* minion;

	CardState state = CardState::IDLE;
	int initY = 0;

	void AssignButton()
	{
		const SDL_Rect* relTF = GetParent()->GetRelTf();
		this->initY = relTF->y;

		AddOnHoverEnter([this, relTF] {
			if (state == CardState::IDLE)
			{
				GetParent()->SetRelPosition(relTF->x, relTF->y - 5);
				state = CardState::HOVERED;
			}
			});

		AddOnLeftClick([this, relTF] {
			if (state == CardState::HOVERED)
			{
				GetParent()->SetRelPosition(relTF->x, relTF->y - 15);
				state = CardState::CHOSEN;
			}
			});

		AddOnHoverExit([this] {
			if (state == CardState::HOVERED)
			{
				Deselect();
			}
			});
	}
};