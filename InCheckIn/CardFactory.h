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
		Minion* minion = new Minion(2, 0, "Lefty");
		minion->SetTrigger([minion](Field* field, int index) {
			std::cout << minion->GetName() << ": Repeating Left" << std::endl;
			if (index - 1 >= 0) field->TriggerCard(index - 1);
			});

		return NewCard(Conf::CARD_IMAGE_LEFTY, minion);
	}

	static std::unique_ptr<GameObject> Basic()
	{
		Minion* minion = new Minion(0, 0, "Basic");
		minion->SetTrigger([minion](Field* field, int index) {
			std::cout << minion->GetName() << ": Do Something Basic" << std::endl;
			});

		return NewCard(Conf::CARD_IMAGE_BASIC, minion);
	}

private:
	static std::unique_ptr<GameObject> NewCard(const std::string& imagePath, Minion* minion)
	{
		auto cardObj = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
		Card* card = new Card(cardObj.get(), minion);
		Image* image = new Image(cardObj.get(), imagePath);

		Text* text;
		cardObj->AdoptChild(std::move(UIFactory::NewText(
			10, Conf::CARD_HEIGHT - 30, Conf::CARD_WIDTH / 5, 20,
			text)));
		text->SetText(std::to_string(minion->GetSanity()), Conf::SANITY_COLOR);

		cardObj->AddComponent(image);
		cardObj->AddComponent(card);
		return cardObj;
	}
};