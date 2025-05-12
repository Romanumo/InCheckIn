#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "GameManager.h"
#include "CardFactory.h"
#include "PopUpManager.h"
#include "Deck.h"
#include "Card.h"
using namespace Engine;

class Shop : public GameObject
{
public:
	Shop(Deck* deck, std::function<void()> onPickUp) : 
		GameObject(0, 0, 500, 0), deck(deck), nextScene(onPickUp)
	{
		Layout* row = new Layout(this, new Row(), Conf::PADDING, 0);

		row->AddGameObject(std::move(CreateCard(CardFactory::GetPlayerCard())));
		row->AddGameObject(std::move(CreateCard(CardFactory::GetPlayerCard())));
		row->AddGameObject(std::move(CreateCard(CardFactory::GetPlayerCard())));
		AddComponent(row);
	}

	void NewShop()
	{
		for (auto card : cards)
		{
			CardStats stats = CardFactory::GetPlayerCard();

			card.stats = stats;
			card.image->SetImage(stats.imagePath);
			card.price->SetText(std::to_string(stats.spiralCost));

			card.button->ClearRightClick();
			card.button->ClearLeftClick();

			card.button->AddOnRightClick([this, stats, cr = card.image->GetParent()] {
				const SDL_Rect* absTF = cr->GetAbsTf();
				PopUpManager::GetInstance().CallHint(absTF->x + absTF->w, absTF->y,
					stats.minionStats.name, stats.minionStats.desc);
				});

			card.button->AddOnLeftClick([this, stats] {
				deck->AddCard(stats);
				if (nextScene) nextScene();
				NewShop();
				});
		}
	}

private:
	std::vector<CardUI> cards;
	std::function<void()> nextScene;
	Deck* deck;

	//Use New card. Store cards themselves. Change their statuses
	std::unique_ptr<GameObject> CreateCard(CardStats stats)
	{
		CardUI UI;

		auto card = CardFactory::NewCard(stats, &UI);
		card->RemoveComponent<Card>();
		Button* button = new Button(card.get());
		UI.button = button;

		const SDL_Rect* relTF = card->GetRelTf();

		button->AddOnHoverEnter([this, relTF, cr = card.get()] {
			cr->SetRelPosition(relTF->x, relTF->y - 5);
			});

		button->AddOnRightClick([this, stats, cr = card.get()] {
			const SDL_Rect* absTF = cr->GetAbsTf();
			PopUpManager::GetInstance().CallHint(absTF->x + absTF->w, absTF->y,
				stats.minionStats.name, stats.minionStats.desc);
			});

		button->AddOnHoverExit([this, relTF, cr = card.get()] {
			cr->SetRelPosition(relTF->x, relTF->y + 5);
			PopUpManager::GetInstance().HideHint();
			});

		button->AddOnLeftClick([this, stats] {
			deck->AddCard(stats);
			if (nextScene) nextScene();
			NewShop();
			});

		card->AddComponent(button);
		cards.push_back(UI);
		return card;
	}
};