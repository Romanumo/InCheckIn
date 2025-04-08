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

        AddCard(CardFactory::NewCard(CardFactory::Lefty()));
        AddCard(CardFactory::NewCard(CardFactory::Obsession()));
        AddCard(CardFactory::NewCard(CardFactory::Righty()));
        AddCard(CardFactory::NewCard(CardFactory::Basic()));
        AddCard(CardFactory::NewCard(CardFactory::Basic()));

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
        if (cards.size() >= 6 || !cardObj) return;

        Card* card = cardObj->GetComponent<Card>();
        card->AddOnLeftClick(
            [this, card] {
                if (card->GetState() == CardState::CHOSEN)
                {
                    ChooseCard(card);
                }
            });

        cards.push_back(card);

        rowComponent->AddGameObject(std::move(cardObj), true);
    }
    
    std::unique_ptr<GameObject> PlaceCard()
    {
        if (!chosenCard || !isEnabled) return nullptr;

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
    
    void SetEnabled(bool enable) { isEnabled = enable; }

private:
    std::vector<Card*> cards;
    bool isEnabled = true;

    Card* chosenCard = nullptr;
    Layout* rowComponent = nullptr;

    void RemoveCard(Card* card)
    {
        auto it = std::find(cards.begin(), cards.end(), chosenCard);
        if (card == chosenCard) chosenCard = nullptr;
        if (it != cards.end()) cards.erase(it);
    }
};