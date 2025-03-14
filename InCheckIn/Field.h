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

    void SetOpposingField(Field* opposing)
    {
        if (opposingField) return;

        opposingField = opposing;
        opposing->SetOpposingField(this);
    }

private:
    Field* opposingField;
    Text* sanityText;
    int sanity;

    Card* cardPlaced[Conf::MAX_CARDS];

    void CreateSlots(Hand* hand)
    {
        auto slots = UIFactory::GetLayout<GameObject>(this, new Row(), Conf::MAX_CARDS,
            0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);

        int index = 0;
        for (const auto slot : slots)
        {
            Button* button = new Button(slot);
            Image* image = new Image(slot, Conf::PLACEHOLDER_IMAGE);

            if (hand) AssignHand(hand, button, slot, index);

            slot->AddComponent(button);
            slot->AddComponent(image);
            index++;
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

    void AssignHand(Hand* hand, Button* button, GameObject* slot, int index)
    {
        button->AddOnLeftClick([this, hand, button, slot, index] {
            std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
            if (cardOriginal)
            {
                GameObject* cardRef = cardOriginal.get();
                slot->AdoptChild(std::move(cardOriginal));
                cardRef->SetRelPosition(0, 0);

                button->SetEnabled(false);
                cardPlaced[index] = dynamic_cast<Card*>(cardRef);
            }
        });
    }
};
