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

		enemyCards[CardFactory::Null] = 100;
		enemyCards[CardFactory::Sun] = 0;
		enemyCards[CardFactory::Hobby] = 0;
		enemyCards[CardFactory::Friends] = 0;

		GM.AddOnNewGame([this, GM](int level) {
			if (level == 0) return;
			else if (level == 1)
			{
				enemyCards[CardFactory::Sun] = 70;
				enemyCards[CardFactory::Hobby] = 25;
				enemyCards[CardFactory::Friends] = 3;
			}
			else if(level > 1) IncreaseDifficulty(level);
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
		if(stats.isValid()) field->PlaceCard(CardFactory::NewCard(stats), slot);
	}

private:
	std::unordered_map<CardStats(*)(), int> enemyCards;
	Field* field;

	void IncreaseDifficulty(int level)
	{
		if(enemyCards[CardFactory::Null] > 25) enemyCards[CardFactory::Null] -= 5;
		enemyCards[CardFactory::Sun] += 3;
		enemyCards[CardFactory::Hobby] += 4;
		enemyCards[CardFactory::Friends] += 5;
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

		std::cout << "No Chance card was given. Return Null" << std::endl;
		return CardFactory::Null();
	}
};