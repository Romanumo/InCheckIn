#pragma once 
#include "Engine/UIFactory.h"
#include "Engine/Component.h"
#include "Engine/Managers/Random.h"
#include "Field.h"
#include "Card.h"

static class CardFactory
{
public:
	static CardStats Lefty()
	{
		return CardStats(Conf::CARD_IMAGE_LEFTY, 4,
			MinionStats("Lefty", [](Minion* self, int index) -> bool {
				Minion* triggerMinion = self->GetField()->GetMinionAt(index - 1);
				if (index - 1 >= 0 && triggerMinion)
				{
					if (triggerMinion->GetName() == "Righty" && Random::Chance(40)) return true;

					self->GetField()->TriggerCard(index - 1);
					return false;
				}
				return true;
			}));
	}

	static CardStats Righty()
	{
		return CardStats(Conf::CARD_IMAGE_RIGHTY, 4,
			MinionStats("Righty", [](Minion* self, int index) -> bool {
				Minion* triggerMinion = self->GetField()->GetMinionAt(index + 1);
				if (index + 1 <= Conf::MAX_CARDS && triggerMinion)
				{
					if (triggerMinion->GetName() == "Lefty" && Random::Chance(40)) return true;

					self->GetField()->TriggerCard(index + 1);
					return false;
				}
				return true;
				}));
	}

	static CardStats Obsession()
	{
		return CardStats(Conf::CARD_IMAGE_OBSESSION, 1,
			MinionStats("Obsession", [](Minion* self, int index) -> bool {
				if (Random::Chance(30)) return true;

				int slot = Random::Int(0, Conf::MAX_CARDS - 1);
				if (self->GetField()->GetMinionAt(slot))
				{
					self->GetField()->TriggerCard(slot);
					return false;
				}

				return true;
				}));
	}

	static CardStats Basic()
	{
		return CardStats(Conf::CARD_IMAGE_BASIC, 0,
			MinionStats("Basic", [](Minion* self, int index) -> bool {
				return true;
			}));
	}

	static CardStats Sun()
	{
		return CardStats(Conf::CARD_IMAGE_SUN, 0,
			MinionStats("Sun", [](Minion* self, int index) -> bool {
				self->GetField()->ChangeSpiralCombo(-3);
				return true;
				}));
	}

	static std::unique_ptr<GameObject> NewCard(CardStats stats)
	{
		if (stats.isEmpty()) return nullptr;

		auto cardObj = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
		cardObj->AddComponent(new Card(cardObj.get(), stats.spiralCost, stats.minionStats));
		cardObj->AddComponent(new Image(cardObj.get(), stats.imagePath));

		return cardObj;
	}
};