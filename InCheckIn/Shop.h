#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "GameManager.h"
#include "CardFactory.h"
#include "HintManager.h"
#include "Card.h"
using namespace Engine;

class Shop : public GameObject
{
public:
	Shop() : GameObject(0,0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT)
	{
		Layout* row = new Layout(this, new Engine::Row(), 0, 0);

		row->AddGameObject(std::move(CreateCard(CardFactory::Basic())));
		row->AddGameObject(std::move(CreateCard(CardFactory::Basic())));
		row->AddGameObject(std::move(CreateCard(CardFactory::Basic())));
	}

private:
	std::unique_ptr<GameObject> CreateCard(CardStats stats)
	{
		auto card = std::make_unique<GameObject>();
		card->AddComponent(new Image(this, stats.imagePath));
		Button* button = new Button(this);

		const SDL_Rect* relTF = GetRelTf();
		int* initY = new int(relTF->y);

		button->AddOnHoverEnter([this, relTF] {
			SetRelPosition(relTF->x, relTF->y - 5);
			});

		button->AddOnRightClick([this, stats] {
			const SDL_Rect* absTF = GetAbsTf();
			HintManager::GetInstance().CallHint(absTF->x + absTF->w, absTF->y,
				stats.minionStats.name, stats.minionStats.desc);
			});

		button->AddOnHoverExit([this, relTF, initY] {
			SetRelPosition(relTF->x, *initY);
			HintManager::GetInstance().HideHint();
			});

		button->AddOnLeftClick([this, stats] {
			GameManager::GetDeck()->AddCard(stats);
			GameManager::SwitchToGame();
			});

		card->AddComponent(button);
		return card;
	}
};