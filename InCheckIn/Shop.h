#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "GameManager.h"
#include "CardFactory.h"
#include "HintManager.h"
#include "Deck.h"
#include "Card.h"
using namespace Engine;

class Shop : public GameObject
{
public:
	Shop(Deck* deck, std::function<void()> onPickUp) : GameObject(0, 0, 500, 0)
	{
		Layout* row = new Layout(this, new Row(), Conf::PADDING, 0);

		row->AddGameObject(std::move(CreateCard(CardFactory::Basic(), deck, onPickUp)));
		row->AddGameObject(std::move(CreateCard(CardFactory::Basic(), deck, onPickUp)));
		row->AddGameObject(std::move(CreateCard(CardFactory::Basic(), deck, onPickUp)));
		AddComponent(row);
	}

private:
	std::unique_ptr<GameObject> CreateCard(CardStats stats, Deck* deck, std::function<void()> onPickUp)
	{
		auto card = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
		card->AddComponent(new Image(card.get(), stats.imagePath));
		Button* button = new Button(card.get());

		const SDL_Rect* relTF = card->GetRelTf();

		button->AddOnHoverEnter([this, relTF, cr = card.get()] {
			cr->SetRelPosition(relTF->x, relTF->y - 5);
			});

		button->AddOnRightClick([this, stats, cr = card.get()] {
			const SDL_Rect* absTF = cr->GetAbsTf();
			HintManager::GetInstance().CallHint(absTF->x + absTF->w, absTF->y,
				stats.minionStats.name, stats.minionStats.desc);
			});

		button->AddOnHoverExit([this, relTF, cr = card.get()] {
			cr->SetRelPosition(relTF->x, relTF->y + 5);
			HintManager::GetInstance().HideHint();
			});

		button->AddOnLeftClick([this, stats, deck, onPickUp] {
			deck->AddCard(stats);
			if(onPickUp) onPickUp();
			});

		card->AddComponent(button);
		return card;
	}
};