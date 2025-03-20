#pragma once
#include "Engine/UIFactory.h"
#include "Engine/Button.h"
#include "Engine/GameObject.h"
#include "CardFactory.h"
#include "Card.h"

class Hand : public GameObject
{
public:
    Hand() : GameObject()
	{
        rowComponent = new Layout(this, new Row(), Conf::PADDING, 0);
        for (int i = 0;i < 5;i++)
        {
            auto card = std::make_unique<Card>(CardFactory::Lefty());

            card->GetComponent<Button>()->AddOnLeftClick(
                [card = card.get(), this] {
                if (card->GetState() == CardState::CHOSEN)
                {
                    ChooseCard(card);
                }
                });

            this->cards.push_back(card.get());
            rowComponent->AddGameObject(std::move(card));
        }
        this->SetRelSize(Conf::TABLE_WIDTH, Conf::CARD_HEIGHT);
        rowComponent->AlignCenter();
        
        AddComponent(rowComponent);
	}

    void ChooseCard(Card* card)
    {
        if (chosenCard && chosenCard != card) chosenCard->Deselect();

        chosenCard = card;
    }

    std::unique_ptr<Card> PlaceCard()
    {
        if (!chosenCard) return nullptr;

        std::unique_ptr<GameObject> child = TransferChild(chosenCard);
        RemoveCard(chosenCard);

        chosenCard = nullptr;
        rowComponent->AlignCenter();
        return std::unique_ptr<Card>(dynamic_cast<Card*>(child.release()));
    }
    
private:
    std::vector<Card*> cards;

    Card* chosenCard = nullptr;
    Layout* rowComponent = nullptr;

    void RemoveCard(Card* card)
    {
        auto it = std::find(cards.begin(), cards.end(), chosenCard);
        if (it != cards.end()) cards.erase(it);
    }
};