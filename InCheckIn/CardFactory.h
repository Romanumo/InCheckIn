#pragma once
#include "Engine/UIFactory.h"
#include "Engine/Component.h"
#include "CardStats.h"

static class CardFactory
{
public:
	static CardStats Lefty()
	{
		return CardStats("Lefty", Conf::CARD_IMAGE_LEFTY, 0, 2, NULL);
	}
};