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
			MinionStats("Rumination", "Triggers card to the left (Compulsion with 70%)",
				[](Minion* self, int index) -> bool {
				Minion* triggerMinion = self->GetField()->GetMinionAt(index - 1);
				if (index - 1 >= 0 && triggerMinion)
				{
					if (triggerMinion->GetName() == "Compulsion" && Random::Chance(30)) return true;

					self->GetField()->TriggerCard(index - 1);
					return false;
				}
				return true;
			}));
	}

	static CardStats Righty()
	{
		return CardStats(Conf::CARD_IMAGE_RIGHTY, 4,
			MinionStats("Compulsion", "Triggers card to the right (Rumination with 70%)", 
				[](Minion* self, int index) -> bool {
				Minion* triggerMinion = self->GetField()->GetMinionAt(index + 1);
				if (index + 1 <= Conf::MAX_CARDS && triggerMinion)
				{
					if (triggerMinion->GetName() == "Rumination" && Random::Chance(30)) return true;

					self->GetField()->TriggerCard(index + 1);
					return false;
				}
				return true;
				}));
	}

	static CardStats Repeater()
	{
		return CardStats(Conf::CARD_IMAGE_OBSESSION, 1,
			MinionStats("Mental Check", "Triggers random card with 70%", 
				[](Minion* self, int index) -> bool {
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
			MinionStats("Thought", "Does Nothing",
				[](Minion* self, int index) -> bool {
				return true;
			}));
	}

	static CardStats Sun()
	{
		std::shared_ptr<int> turns = std::make_shared<int>(0);
		return CardStats(Conf::CARD_IMAGE_SUN, 0,
			MinionStats("Sun", "Deducts 2 spiral",
				[turns](Minion* self, int index) -> bool {

				(*turns)++;
				std::cout << "Turns Made " << (*turns) << std::endl;
				if(*turns <= 1) return true;

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