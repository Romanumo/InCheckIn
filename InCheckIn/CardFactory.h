#pragma once 
#include <unordered_map>
#include <functional>
#include "Engine/UIFactory.h"
#include "Engine/Component.h"
#include "Engine/Managers/Random.h"
#include "HintManager.h"
#include "Field.h"
#include "Card.h"

class CardUI
{
public:
	CardUI() {}
	CardUI(Image* image, Text* price, Button* button, CardStats stats)
	{
		this->image = image;
		this->button = button;
		this->price = price;
		this->stats = stats;
	}

	Image* image;
	Text* price;
	Button* button;
	CardStats stats;
};

static class CardFactory
{
public:
	static void Init()
	{
		playerCards[Lefty] = 15;
		playerCards[Righty] = 15;
		playerCards[Repeater] = 15;
		playerCards[Firstly] = 20;
		playerCards[Dread] = 20;
		playerCards[Basic] = 20;

		enemyCards[Sun] = 50;
		enemyCards[Hobby] = 30;
		enemyCards[Friends] = 20;
	}

	static CardStats GetPlayerCard()
	{
		int totalChance = 0;
		for (auto card : playerCards) totalChance += card.second;

		int roll = Random::Int(0, totalChance);
		int cumulative = 0;

		for (auto card : playerCards)
		{
			cumulative += card.second;
			if (cumulative >= roll) return card.first();
		}
		
		std::cout << "No Chance card was given. Return Basic" << std::endl;
		return Basic();
	}

	static CardStats GetEnemyCard()
	{
		int totalChance = 0;
		for (auto card : enemyCards) totalChance += card.second;

		int roll = Random::Int(0, totalChance);
		int cumulative = 0;

		for (auto card : enemyCards)
		{
			cumulative += card.second;
			if (cumulative > roll) return card.first();
		}

		std::cout << "No Chance card was given. Return Sun" << std::endl;
		return Sun();
	}

	static CardStats Lefty()
	{
		return CardStats(Conf::CARD_IMAGE_LEFTY, 7,
			MinionStats("Rumination", "Triggers card to the left (Compulsion with 75%)",
				[](Minion* self, int index) -> bool {
				Minion* triggerMinion = self->GetField()->GetMinionAt(index - 1);
				if (index - 1 >= 0 && triggerMinion)
				{
					if (triggerMinion->GetName() == "Compulsion" && Random::Chance(25)) return true;

					self->GetField()->TriggerCard(index - 1);
					return false;
				}
				return true;
			}));
	}

	static CardStats Righty()
	{
		return CardStats(Conf::CARD_IMAGE_RIGHTY, 7,
			MinionStats("Compulsion", "Triggers card to the right (Rumination with 75%)", 
				[](Minion* self, int index) -> bool {
				Minion* triggerMinion = self->GetField()->GetMinionAt(index + 1);
				if (index + 1 <= Conf::MAX_CARDS && triggerMinion)
				{
					if (triggerMinion->GetName() == "Rumination" && Random::Chance(25)) return true;

					self->GetField()->TriggerCard(index + 1);
					return false;
				}
				return true;
				}));
	}

	static CardStats Repeater()
	{
		return CardStats(Conf::CARD_IMAGE_OBSESSION, 10,
			MinionStats("Mental Check", "Triggers random card (Itself with 50%)", 
				[](Minion* self, int index) -> bool {
				int slot = Random::Int(0, Conf::MAX_CARDS - 1);
				while (!self->GetField()->GetMinionAt(slot))
				{
					slot = Random::Int(0, Conf::MAX_CARDS - 1);
				}

				if (slot == index && Random::Chance(50)) return true;
				
				self->GetField()->TriggerCard(slot);
				return false;
				}));
	}

	static CardStats Dread()
	{
		return CardStats(Conf::CARD_IMAGE_PROTOTYPE, 0,
			MinionStats("Dread", "Gives 5 Spiral and then Dies",
				[](Minion* self, int index) -> bool {
					self->GetField()->ChangeSpiralCombo(4);
					self->GetParent()->SetActive(false);
					return true;
				}));
	}

	static CardStats Firstly()
	{
		return CardStats(Conf::CARD_IMAGE_PROTOTYPE, 5,
			MinionStats("Firstly", "Triggers first card (Compulsion with 50%. Cant Trigger itself)",
				[](Minion* self, int index) -> bool {
					Minion* triggerMinion = self->GetField()->GetMinionAt(0);
					if (triggerMinion && index != 0)
					{
						if (triggerMinion->GetName() == "Compulsion" && Random::Chance(50)) return true;

						self->GetField()->TriggerCard(0);
						return false;
					}
					return true;
				}));
	}

	static CardStats Basic()
	{
		return CardStats(Conf::CARD_IMAGE_BASIC, 0,
			MinionStats("Thought", "Gives 2 spiral ",
				[](Minion* self, int index) -> bool {
					self->GetField()->ChangeSpiralCombo(1);
					return true;
			}));
	}

	static CardStats Sun()
	{
		std::shared_ptr<int> turns = std::make_shared<int>(1);
		return CardStats(Conf::CARD_IMAGE_SUN, 0,
			MinionStats("Sun", "Activated after " + std::to_string(*turns) + " turn. Deducts 3 spiral",
				[turns](Minion* self, int index) -> bool {return EnemyFunctions(self, index, turns, 3);}
				));
	}

	static CardStats Hobby()
	{
		std::shared_ptr<int> turns = std::make_shared<int>(3);
		return CardStats(Conf::CARD_IMAGE_PROTOTYPE, 0,
			MinionStats("Hobby", "Activated after " + std::to_string(*turns) + " turn. Deducts 10 spiral",
				[turns](Minion* self, int index) -> bool {return EnemyFunctions(self, index, turns, 10);}
			));
	}

	static CardStats Friends()
	{
		std::shared_ptr<int> turns = std::make_shared<int>(5);
		return CardStats(Conf::CARD_IMAGE_PROTOTYPE, 0,
			MinionStats("Friends", "Activated after " + std::to_string(*turns) + " turn. Deducts 15 spiral",
				[turns](Minion* self, int index) -> bool {return EnemyFunctions(self, index, turns, 15);}
			));
	}

	static std::unique_ptr<GameObject> NewCard(CardStats stats, CardUI* cardUI = nullptr)
	{
		if (stats.isEmpty()) return nullptr;

		auto cardObj = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
		Card* card = new Card(cardObj.get(), stats.spiralCost, stats.minionStats);
		cardObj->AddComponent(card);

		Image* image = new Image(cardObj.get(), stats.imagePath);
		cardObj->AddComponent(image);

		auto priceText = std::make_unique<GameObject>(
			Conf::PADDING, Conf::CARD_HEIGHT - Conf::PADDING - 25, 35, 30);
		priceText->AddComponent(new Rectangle(priceText.get()));

		Text* text = new Text(priceText.get(), std::to_string(stats.spiralCost), Conf::SPIRAL_COLOR, 25);
		priceText->AddComponent(text);
		cardObj->AdoptChild(std::move(priceText));

		if (cardUI)
		{
			cardUI->button = card;
			cardUI->image = image;
			cardUI->stats = stats;
			cardUI->price = text;
		}

		return cardObj;
	}

private:
	inline static std::unordered_map<CardStats(*)(), int> playerCards;
	inline static std::unordered_map<CardStats(*)(), int> enemyCards;

	static bool EnemyFunctions(Minion* self, int index, std::shared_ptr<int> turns, int spiralDecrease)
	{
		(*turns)--;
		if (*turns > 0)
		{
			self->ChangeDesc("Activated after " + std::to_string(*turns) + " turn. Deducts " +
				std::to_string(spiralDecrease) + " spiral");
			return true;
		}
		else if (*turns == 0)
		{
			self->ChangeDesc("Activated. Deducts " + std::to_string(spiralDecrease) + " spiral");
			return true;
		}

		self->GetField()->ChangeSpiralCombo(spiralDecrease * -1);
		return true;
	}
};