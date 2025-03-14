#pragma once
#include "Engine/UIFactory.h"
#include "Engine/Component.h"
#include "CardStats.h"

static class CardFactory
{
public:
	static CardStats Healer()
	{
		return CardStats("Healer", Conf::CARD_IMAGE_HEALER, 0, 2, NULL);
	}
};