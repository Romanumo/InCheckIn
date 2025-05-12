#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "HintManager.h"
#include "Minion.h"
using namespace Engine;

enum CardState
{
	IDLE,
	HOVERED,
	CHOSEN
};

struct CardStats
{
	CardStats() { spiralCost = -1; }
	CardStats(const std::string& imagePath, int spiralCost, MinionStats minionStats)
	{
		this->imagePath = imagePath;
		this->spiralCost = spiralCost;
		this->minionStats = minionStats;
	}

	std::string imagePath;
	MinionStats minionStats;
	int spiralCost;

	bool isEmpty() { return spiralCost < 0; }
};

class Hammer;

class Card : public Button
{
public:
	Card(GameObject* parent, int spiralCost, MinionStats stats) : 
		Button(parent), minionStats(stats), spiral(spiralCost)
	{
		AssignButton();
	}

	void TransformToMinion(Field* field, Minion*& minion, Hammer* hammer)
	{
		minion = new Minion(GetParent(), field, minionStats, hammer);
		GetParent()->AddComponent(minion);
		GetParent()->RemoveComponent<Card>();
	}

	void Deselect()
	{
		GetParent()->SetRelPosition(GetParent()->GetRelTf()->x, initY);
		state = CardState::IDLE;
	}

	int GetSpiral() { return spiral; }
	CardState GetState() { return state; }

private:
	int spiral;
	MinionStats minionStats;

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
				HintManager::GetInstance().HideHint();
			}
			});

		AddOnRightClick([this] {
			const SDL_Rect* absTF = GetParent()->GetAbsTf();
			HintManager::GetInstance().CallHint(absTF->x + absTF->w, absTF->y, 
				minionStats.name, minionStats.desc);
			});

		AddOnHoverExit([this] {
			if (state == CardState::HOVERED)
			{
				Deselect();
				HintManager::GetInstance().HideHint();
			}
			});
	}
};