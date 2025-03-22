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
        AddComponent(rowComponent);

        for (int i = 0;i < 3;i++)
        {
            AddCard(std::move(CardFactory::Lefty()));
        }

        AddCard(CardFactory::Basic());
        AddCard(CardFactory::Basic());

        this->SetRelSize(Conf::TABLE_WIDTH, Conf::CARD_HEIGHT);
        rowComponent->AlignCenter();
    }

    void ChooseCard(Card* card)
    {
        if (chosenCard && chosenCard != card) chosenCard->Deselect();

        chosenCard = card;
    }

    void AddCard(std::unique_ptr<GameObject> card)
    {
        Card* cardComp = card->GetComponent<Card>();
        cardComp->AddOnLeftClick(
            [this, cardComp] {
                if (cardComp->GetState() == CardState::CHOSEN)
                {
                    ChooseCard(cardComp);
                }
            });

        cards.push_back(cardComp);

        rowComponent->AddGameObject(std::move(card));
        rowComponent->AlignCenter();
    }
    
    std::unique_ptr<GameObject> PlaceCard()
    {
        if (!chosenCard) return nullptr;

        std::unique_ptr<GameObject> child = TransferChild(chosenCard->GetParent());
        RemoveCard(chosenCard);

        chosenCard = nullptr;
        rowComponent->AlignCenter();
        return child;
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