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
            AddCard(CardFactory::Lefty());
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

    void AddCard(std::unique_ptr<GameObject> cardObj)
    {
        if (cards.size() >= 6) return;

        Card* card = cardObj->GetComponent<Card>();
        card->AddOnLeftClick(
            [this, card] {
                if (card->GetState() == CardState::CHOSEN)
                {
                    ChooseCard(card);
                }
            });

        cards.push_back(card);

        rowComponent->AddGameObject(std::move(cardObj));
        rowComponent->AlignCenter();
    }
    
    //TODO: Prevent Card from playing if not chosing
    std::unique_ptr<GameObject> PlaceCard()
    {
        if (!chosenCard) return nullptr;

        std::unique_ptr<GameObject> child = TransferChild(chosenCard->GetParent());
        RemoveCard(chosenCard);

        rowComponent->AlignCenter();
        return child;
    }

    int GetChosenCardSpiral()
    {
        if (!chosenCard) return 99999;
        return chosenCard->GetSpiral();
    }
    
private:
    std::vector<Card*> cards;

    Card* chosenCard = nullptr;
    Layout* rowComponent = nullptr;

    void RemoveCard(Card* card)
    {
        auto it = std::find(cards.begin(), cards.end(), chosenCard);
        if (card == chosenCard) chosenCard = nullptr;
        if (it != cards.end()) cards.erase(it);
    }
};