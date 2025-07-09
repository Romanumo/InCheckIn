#pragma once
#include <vector>
#include "Engine/Managers/Random.h"
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "CardFactory.h"
#include "GameManager.h"

class Deck
{
public:
	Deck()
	{
		cards.push_back(CardFactory::Basic());
		cards.push_back(CardFactory::Basic());
		cards.push_back(CardFactory::Dread());
		cards.push_back(CardFactory::Dread());

		//cards.push_back(CardFactory::Spiral());
		//cards.push_back(CardFactory::Firstly());
		//cards.push_back(CardFactory::Basic());
		//cards.push_back(CardFactory::Repeater());
		//cards.push_back(CardFactory::Righty());
		//cards.push_back(CardFactory::Lefty());
	}

	void ConnectToGM(GameManager& GM)
	{
		GM.AddOnNewGame([this] {
			Shuffle();
			this->index = 0;
			});
	}

	CardStats GetCard()
	{
		if (index >= cards.size()) return CardStats();
		index++;
		return cards.at(index - 1);
	}

	void Shuffle()
	{
		for (int i = 0;i < cards.size();i++)
		{
			int j = Random::Int(0, cards.size() - 1);
			std::swap(cards[i], cards[j]);
		}
	}

	void AddCard(CardStats stats)
	{
		SoundManager::GetInstance().PlaySFX(Conf::SOUND_NEWCARD);
		cards.push_back(stats);
		onNewCard.Invoke();
	}

	void AddOnNewCard(std::function<void()> event) { onNewCard.AddEvent(event); }

private:
	std::vector<CardStats> cards;
	Event onNewCard = Event();
	unsigned int index = 0;
};