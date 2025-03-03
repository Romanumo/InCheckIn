#pragma once
#include "Engine/UIFactory.h"
#include "Engine/Button.h"
#include "Engine/GameObject.h"
#include "Card.h"

class CardHand : public Engine::GameObject
{
public:
    CardHand(int x, int y) : Engine::GameObject(x, y, 0, 0)
	{
        std::vector<Engine::GameObject*> cards;
        for (int i = 0;i < 5;i++)
        {
            Card* card = new Card(0, 0);

            card->GetComponent<Engine::Button>()->AddOnLeftClick([card, this] {
                if (card->GetState() == CardState::CHOSEN)
                {
                    ChooseCard(card);
                }
                });

            this->cards.push_back(card);
            cards.push_back(card);
        }
        Engine::Row* rowComponent = new 
            Engine::Row(this, Config::PADDING, 0, cards);
        this->SetRelSize((Config::CARD_WIDTH + Config::PADDING) * Config::SIDE_MAX_CARDS,
            Config::CARD_HEIGHT);
        rowComponent->AlignOnCenter();

        AddComponent(rowComponent);
	}

    void ChooseCard(Card* card)
    {
        if (chosenCard && chosenCard != card) chosenCard->Deselect();

        chosenCard = card;
    }

    std::unique_ptr<GameObject> PlaceCard()
    {
        if (!chosenCard) return nullptr;

        //Hand needs to move ownership of card to the tableSide
        std::unique_ptr<GameObject> child = TransferChild(chosenCard);

        chosenCard->IntoPlayedState();
        RemoveCard(chosenCard);

        chosenCard = nullptr;
        return child;
    }

private:
    std::vector<Card*> cards;
    Card* chosenCard = nullptr;

    void RemoveCard(Card* card)
    {
        auto it = std::find(cards.begin(), cards.end(), chosenCard);
        if (it != cards.end()) cards.erase(it);
    }
};