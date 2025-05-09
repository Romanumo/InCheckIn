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
		cards = new CardStats * [3]();
		cards[0] = new CardStats(CardFactory::Basic());
		cards[1] = new CardStats(CardFactory::Basic());
		cards[2] = new CardStats(CardFactory::Basic());

		Layout* row = new Layout(this, new Row(), Conf::PADDING, 0);

		row->AddGameObject(std::move(CreateCard(cards[0], deck, onPickUp)));
		row->AddGameObject(std::move(CreateCard(cards[1], deck, onPickUp)));
		row->AddGameObject(std::move(CreateCard(cards[2], deck, onPickUp)));
		AddComponent(row);
	}

	~Shop()
	{
		for (int i = 0;i<3;i++)
		{
			delete cards[i];
		}
		delete cards;
	}

	void NewShop()
	{
		cards[0] = new CardStats(CardFactory::Dread());
		cards[1] = new CardStats(CardFactory::Basic());
		cards[2] = new CardStats(CardFactory::Basic());
	}

private:
	CardStats** cards;

	//Use New card. Store cards themselves. Change their statuses
	std::unique_ptr<GameObject> CreateCard(CardStats* stats, Deck* deck, std::function<void()> onPickUp)
	{
		auto card = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
		card->AddComponent(new Image(card.get(), stats->imagePath));
		Button* button = new Button(card.get());

		const SDL_Rect* relTF = card->GetRelTf();

		button->AddOnHoverEnter([this, relTF, cr = card.get()] {
			cr->SetRelPosition(relTF->x, relTF->y - 5);
			});

		button->AddOnRightClick([this, stats, cr = card.get()] {
			const SDL_Rect* absTF = cr->GetAbsTf();
			HintManager::GetInstance().CallHint(absTF->x + absTF->w, absTF->y,
				stats->minionStats.name, stats->minionStats.desc);
			});

		button->AddOnHoverExit([this, relTF, cr = card.get()] {
			cr->SetRelPosition(relTF->x, relTF->y + 5);
			HintManager::GetInstance().HideHint();
			});

		button->AddOnLeftClick([this, stats, deck, onPickUp] {
			deck->AddCard(*stats);
			if (onPickUp) onPickUp();
			NewShop();
			});

		card->AddComponent(button);
		return card;
	}
};