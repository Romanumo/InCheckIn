#pragma once 
#include "Engine/UIFactory.h"
#include "Engine/Component.h"
#include "Field.h"
#include "Card.h"

static class CardFactory
{
public:
	static std::unique_ptr<GameObject> Lefty()
	{
		return NewCard(Conf::CARD_IMAGE_LEFTY, 2, 
			MinionStats("Lefty", [](Minion* self, int index) -> bool {
			std::cout << self->GetName() << " " << index << ": Repeating Left" << std::endl;
			if (index - 1 >= 0) self->GetField()->TriggerCard(index - 1);
			return false;
			}));
	}

	static std::unique_ptr<GameObject> Basic()
	{
		return NewCard(Conf::CARD_IMAGE_BASIC, 0,
			MinionStats("Basic", [](Minion* self, int index) -> bool {
			std::cout << self->GetName() << " " << index << ": Do Something" << std::endl;
			return true;
			}));
	}

private:
	static std::unique_ptr<GameObject> NewCard(
		const std::string& imagePath, int spiralCost, MinionStats minionStats)
	{
		auto cardObj = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
		cardObj->AddComponent(new Card(cardObj.get(), spiralCost, minionStats));
		cardObj->AddComponent(new Image(cardObj.get(), imagePath));

		cardObj->AdoptChild(std::move(UIFactory::NewText(
			10, Conf::CARD_HEIGHT - 30, Conf::CARD_WIDTH / 5, 20,
			std::to_string(spiralCost), Conf::SPIRAL_COLOR)));

		cardObj->AdoptChild(std::move(UIFactory::NewText(
			Conf::CARD_WIDTH / 3, 20, Conf::CARD_WIDTH / 3, 20,
			minionStats.name)));

		return cardObj;
	}
};