#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "CardHand.h"

class TableSide : public Engine::GameObject
{
public:
    TableSide() : Engine::GameObject(0, 0, 0, 0)
    {
        using namespace Engine;
        //Image* sideImage = new Image(this, Config::DESK_IMAGE);
        //this->AddComponent(sideImage);

        std::vector<GameObject*> cardsPlaceholder;
        for (int i = 0;i < Config::SIDE_MAX_CARDS;i++)
        {
            GameObject* placeholder = new GameObject(0, 0,
                Config::CARD_WIDTH, Config::CARD_HEIGHT);
            Image* image = new Image(placeholder, Config::PLACEHOLDER_IMAGE);
            Button* button = new Button(placeholder);

            placeholder->AddComponent(image);
            placeholder->AddComponent(button);

            cardsPlaceholder.push_back(placeholder);
            placeholdersButtons.push_back(button);
        }

        UIFactory::GetRowComponent(this, cardsPlaceholder);
    }

    void AssignHand(CardHand* hand)
    {
        for (Engine::Button* button : placeholdersButtons)
        {
            button->AddOnLeftClick([hand, button] {
                std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
                GameObject* cardRef = cardOriginal.get();
                
                if (cardOriginal)
                {
                    button->GetParent()->AdoptChild(std::move(cardOriginal));
                    cardRef->SetRelPosition(0, 0);
                }
                });
        }
    }

private:
    std::vector<Engine::Button*> placeholdersButtons;
};