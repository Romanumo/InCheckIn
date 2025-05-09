#pragma once
#include "Engine/UIFactory.h"
#include "Engine/Button.h"
#include "Engine/GameObject.h"
#include "GameManager.h"
#include "CardFactory.h"
#include "Card.h"
#include "Deck.h"

class Hand : public GameObject
{
public:
    Hand(GameManager& GM, Deck& deck) : GameObject()
	{
        rowComponent = new Layout(this, new Row(), Conf::PADDING, 0);
        AddComponent(rowComponent);

        this->SetRelSize(Conf::TABLE_WIDTH, Conf::CARD_HEIGHT);
        rowComponent->AlignCenter();

        GM.AddOnTurnChange([this](GameFlow turn) {
            isEnabled = (turn == GameFlow::CHOOSING);
        });

        GM.AddOnNewGame([this, &deck]() {
            EmptyHand();

            AddCard(CardFactory::NewCard(deck.GetCard()));
            AddCard(CardFactory::NewCard(deck.GetCard()));
            AddCard(CardFactory::NewCard(deck.GetCard()));
            });
    }

    void ChooseCard(Card* card)
    {
        if (chosenCard && chosenCard != card) chosenCard->Deselect();

        chosenCard = card;
    }

    void AddCard(std::unique_ptr<GameObject> cardObj)
    {
        if (cards.size() >= Conf::MAX_HAND_SIZE || !cardObj) return;

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

    void EmptyHand()
    {
        for (auto card : cards)
        {
            RemoveCard(card);
            RemoveChild(card->GetParent());
        }
    }
};