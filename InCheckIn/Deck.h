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
		cards.push_back(CardFactory::Dread());
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
		cards.push_back(stats);
	}

private:
	std::vector<CardStats> cards;
	unsigned int index = 0;
};