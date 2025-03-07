#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "CardHand.h"

class Cell : public Engine::GameObject
{
public:
    Cell(int w, int h) : Engine::GameObject(0, 0, w, h)
	{
        using namespace Engine;
        Image* image = new Image(this, Config::PLACEHOLDER_IMAGE);
        button = new Button(this);

        AddComponent(image);
        AddComponent(button);
	}

    void AssignToPlayer(CardHand* hand)
    {
        button->AddOnLeftClick([hand, this] {
            std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
            GameObject* cardRef = cardOriginal.get();

            if (cardOriginal)
            {
                AdoptChild(std::move(cardOriginal));
                cardRef->SetRelPosition(0, 0);

                button->SetEnabled(false);
                containingCard = dynamic_cast<Card*>(cardRef);
            }
        });
    }

private:
    Card* containingCard = nullptr;
    Engine::Button* button;
};