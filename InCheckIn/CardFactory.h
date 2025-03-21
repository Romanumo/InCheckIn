#pragma once
#include "Engine/UIFactory.h"
#include "Engine/Component.h"
#include "CardStats.h"
#include "Field.h"
#include "Card.h"

static class CardFactory
{
public:
	static CardStats Lefty()
	{
		return CardStats("Lefty", Conf::CARD_IMAGE_LEFTY, 0, 2, []
		(Card* card, Field* field, int index) {
			std::cout << card->GetName() << ": Repeating Left" << std::endl;
			if (index - 1 >= 0) field->TriggerCard(index - 1);
			});
	}

	static CardStats Basic()
	{
		return CardStats("Basic", Conf::CARD_IMAGE_BASIC, 0, 0, []
		(Card* card, Field* field, int index) {
			std::cout << card->GetName() << ": Do Something" << std::endl;
			});
	}
};