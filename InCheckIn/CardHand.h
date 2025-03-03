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
        using namespace Engine;
        std::vector<std::unique_ptr<GameObject>> cards;
        for (int i = 0;i < 5;i++)
        {
            auto card = std::make_unique<Card>(0,0);

            card->GetComponent<Button>()->AddOnLeftClick(
                [card = card.get(), this] {
                if (card->GetState() == CardState::CHOSEN)
                {
                    ChooseCard(card);
                }
                });

            this->cards.push_back(card.get());
            cards.push_back(std::move(card));
        }
        rowComponent = new Row(this, Config::PADDING, 0, std::move(cards));
        this->SetRelSize(Config::TABLE_WIDTH, Config::CARD_HEIGHT);
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

        rowComponent->AlignOnCenter();
        chosenCard = nullptr;
        return child;
    }

private:
    std::vector<Card*> cards;

    Card* chosenCard = nullptr;
    Engine::Row* rowComponent = nullptr;

    void RemoveCard(Card* card)
    {
        auto it = std::find(cards.begin(), cards.end(), chosenCard);
        if (it != cards.end()) cards.erase(it);
    }
};