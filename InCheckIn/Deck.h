#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "CardFactory.h"

class Deck
{
public:
	Deck()
	{
		cards.push_back(CardFactory::Obsession());
		cards.push_back(CardFactory::Lefty());
		cards.push_back(CardFactory::Lefty());
		cards.push_back(CardFactory::Righty());
		cards.push_back(CardFactory::Righty());
		cards.push_back(CardFactory::Basic());
		cards.push_back(CardFactory::Basic());
		Shuffle();
	}

	CardStats GetCard()
	{
		if (index >= cards.size()) return CardStats();
		index++;
		return cards.at(index - 1);
	}

	void Shuffle()
	{

	}

private:
	std::vector<CardStats> cards;
	unsigned int index = 0;
};