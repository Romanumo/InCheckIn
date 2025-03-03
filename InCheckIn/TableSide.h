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

        std::vector<std::unique_ptr<GameObject>> cardsPlaceholder;
        for (int i = 0;i < Config::SIDE_MAX_CARDS;i++)
        {
            auto placeholder = std::make_unique<GameObject>
                (0, 0, Config::CARD_WIDTH, Config::CARD_HEIGHT);
            Image* image = new Image(placeholder.get(), Config::PLACEHOLDER_IMAGE);
            Button* button = new Button(placeholder.get());

            placeholder->AddComponent(image);
            placeholder->AddComponent(button);

            cardsPlaceholder.push_back(std::move(placeholder));
            placeholdersButtons.push_back(button);
        }

        UIFactory::GetRowComponent(this, std::move(cardsPlaceholder));
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