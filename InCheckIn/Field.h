#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"

#include "Hand.h"

class Field : public GameObject
{
public:
    Field(const std::string& name, const std::string& imagePath,
        Hand* hand = nullptr) : GameObject()
    {
        CreateSlots(hand);
        CreateAvatar(name, imagePath);
    }

private:
    Text* sanityText;
    int sanity;

    Card* cardPlaced[Conf::MAX_CARDS];

    void CreateSlots(Hand* hand)
    {
        auto slots = UIFactory::GetLayout<GameObject>(this, new Row(), Conf::MAX_CARDS,
            0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);

        for (const auto slot : slots)
        {
            Button* button = new Button(slot);
            Image* image = new Image(slot, Conf::PLACEHOLDER_IMAGE);

            if (hand) AssignHand(hand, button, slot);

            slot->AddComponent(button);
            slot->AddComponent(image);
        }
    }

    void CreateAvatar(const std::string& name, const std::string& imagePath)
    {
        int w = Conf::AVATAR_WIDTH;
        int h = Conf::AVATAR_HEIGHT;

        auto avatar = std::make_unique<GameObject>(0, 0, w, h);
        Image* image = new Image(avatar.get(), imagePath);

        avatar->AdoptChild(std::move(UIFactory::NewText(0, 10, w, 30, name)));

        avatar->AdoptChild(std::move(UIFactory::NewText(10, h - 30, w / 5, 20, sanityText)));
        sanityText->SetText(std::to_string(sanity), Conf::SANITY_COLOR);

        avatar->AddComponent(image);

        GetComponent<Layout>()->AddGameObject(std::move(avatar));
    }

    void AssignHand(Hand* hand, Engine::Button* button, GameObject* slot)
    {
        button->AddOnLeftClick([hand, button, slot] {
            std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
            GameObject* cardRef = cardOriginal.get();

            if (cardOriginal)
            {
                slot->AdoptChild(std::move(cardOriginal));
                cardRef->SetRelPosition(0, 0);

                button->SetEnabled(false);
            }
        });
    }
};
