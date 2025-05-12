#pragma once
#include "Engine/Managers/Random.h"
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "GameManager.h"
#include "CardFactory.h"
#include "PopUpManager.h"
#include "Field.h"

class EnemyAI
{
public:
	EnemyAI(GameManager& GM, Field* field)
	{
		this->field = field;

		GM.AddOnNewGame([this, GM](int level) {
			IncreaseDifficulty(level);
		});

		GM.AddOnTurnChange([this] (GameFlow turn){
			if (turn == GameFlow::CHOOSING) PlaceCard();
		});
	}

	void PlaceCard()
	{
		if (field->IsFull()) return;

		int slot = Random::Int(0, Conf::MAX_CARDS - 1);
		while (field->GetMinionAt(slot))
		{
			slot = Random::Int(0, Conf::MAX_CARDS - 1);
		}

		CardStats stats = GetEnemy();
		if(stats.isValid())field->PlaceCard(CardFactory::NewCard(stats), slot);
	}

private:
	std::unordered_map<CardStats(*)(), int> enemyCards;
	Field* field;

	void IncreaseDifficulty(int level)
	{
		if (level < 1)
		{
			enemyCards[CardFactory::Null] = 100;
			enemyCards[CardFactory::Sun] = 0;
			enemyCards[CardFactory::Hobby] = 0;
			enemyCards[CardFactory::Friends] = 0;
		}
		else if (level >= 1)
		{
			enemyCards[CardFactory::Null] = 20;
			enemyCards[CardFactory::Sun] = 50;
			enemyCards[CardFactory::Hobby] = 10;
			enemyCards[CardFactory::Friends] = 10;
		}
	}

	CardStats GetEnemy()
	{
		int totalChance = 0;
		for (auto card : enemyCards) totalChance += card.second;

		int roll = Random::Int(0, totalChance);
		int cumulative = 0;

		for (auto card : enemyCards)
		{
			cumulative += card.second;
			if (cumulative > roll) return card.first();
		}

		std::cout << "No Chance card was given. Return Sun" << std::endl;
		return CardFactory::Sun();
	}
};