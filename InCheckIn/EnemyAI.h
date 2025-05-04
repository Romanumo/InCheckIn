#pragma once
#include "Engine/Managers/Random.h"
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "CardFactory.h"
#include "Field.h"

class EnemyAI
{
public:
	EnemyAI(Field* field)
	{
		this->field = field;
	}

	void PlaceCard()
	{
		if (field->IsFull()) return;

		int slot = Random::Int(0, Conf::MAX_CARDS - 1);
		while (field->GetMinionAt(slot))
		{
			slot = Random::Int(0, Conf::MAX_CARDS - 1);
		}

		if (Random::Chance(20)) field->PlaceCard(CardFactory::NewCard(CardFactory::Hobby()), slot);
		else if (Random::Chance(10)) field->PlaceCard(CardFactory::NewCard(CardFactory::Friends()), slot);
		else if (Random::Chance(50)) field->PlaceCard(CardFactory::NewCard(CardFactory::Sun()), slot);
		else return;
	}

private:
	Field* field;
};