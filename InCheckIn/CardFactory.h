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
			MinionStats("Lefty", [](Minion* self, Field* field, int index) {
			std::cout << self->GetName() << ": Repeating Left" << std::endl;
			if (index - 1 >= 0) field->TriggerCard(index - 1);
			}));
	}

	static std::unique_ptr<GameObject> Basic()
	{
		return NewCard(Conf::CARD_IMAGE_BASIC, 0,
			MinionStats("Basic", [](Minion* self, Field* field, int index) {
			std::cout << self->GetName() << ": Do Something" << std::endl;
			}));
	}

private:
	static std::unique_ptr<GameObject> NewCard(
		const std::string& imagePath, int spiralCost, MinionStats minionStats)
	{
		auto cardObj = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
		Card* card = new Card(cardObj.get(), spiralCost, minionStats);
		Image* image = new Image(cardObj.get(), imagePath);

		Text* text;
		cardObj->AdoptChild(std::move(UIFactory::NewText(
			10, Conf::CARD_HEIGHT - 30, Conf::CARD_WIDTH / 5, 20,
			text)));
		text->SetText(std::to_string(spiralCost), Conf::SPIRAL_COLOR);

		cardObj->AddComponent(image);
		cardObj->AddComponent(card);
		return cardObj;
	}
};