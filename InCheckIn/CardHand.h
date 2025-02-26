#pragma once
#include "Engine/UIFactory.h"
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

private:
    std::vector<Card*> cards;
};