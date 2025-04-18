#pragma once 
#include "Engine/UIFactory.h"
#include "Engine/Component.h"
#include "Field.h"
#include "Card.h"

static class CardFactory
{
public:
	static CardStats Lefty()
	{
		return CardStats(Conf::CARD_IMAGE_LEFTY, 3,
			MinionStats("Lefty", [](Minion* self, int index) -> bool {
				if (index - 1 >= 0 && self->GetField()->GetMinionAt(index - 1))
				{
					self->GetField()->TriggerCard(index - 1);
					return false;
				}
				return true;
			}));
	}

	static CardStats Righty()
	{
		return CardStats(Conf::CARD_IMAGE_PROTOTYPE, 3,
			MinionStats("Righty", [](Minion* self, int index) -> bool {
				if (index + 1 <= Conf::MAX_CARDS &&
					self->GetField()->GetMinionAt(index + 1))
				{
					self->GetField()->TriggerCard(index + 1);
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

	static CardStats Obsession()
	{
		auto addingSpiral = std::make_shared<int>(0);
		return CardStats(Conf::CARD_IMAGE_PROTOTYPE2, 1,
			MinionStats("Obsession", [addingSpiral] (Minion* self, int index) -> bool {
				(*addingSpiral)++;
				self->GetField()->ChangeSpiral(*addingSpiral);
				return true;
				}));
	}

	static CardStats Fun()
	{
		return CardStats(Conf::CARD_IMAGE_PROTOTYPE, 0,
			MinionStats("Fun", [](Minion* self, int index) -> bool {
				self->GetField()->ChangeSpiral(-1);
				return true;
				}));
	}

	static std::unique_ptr<GameObject> NewCard(CardStats stats)
	{
		if (stats.isEmpty()) return nullptr;

		auto cardObj = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
		cardObj->AddComponent(new Card(cardObj.get(), stats.spiralCost, stats.minionStats));
		cardObj->AddComponent(new Image(cardObj.get(), stats.imagePath));

		/*cardObj->AdoptChild(std::move(UIFactory::NewText(
			10, Conf::CARD_HEIGHT - 30, Conf::CARD_WIDTH / 5, 20,
			std::to_string(spiralCost), Conf::SPIRAL_COLOR)));*/

		/*cardObj->AdoptChild(std::move(UIFactory::NewText(
			Conf::CARD_WIDTH / 3, Conf::CARD_HEIGHT * 5/6, Conf::CARD_WIDTH / 3, 20,
			minionStats.name, {0,0,0,255}, 14)));*/

		return cardObj;
	}
};