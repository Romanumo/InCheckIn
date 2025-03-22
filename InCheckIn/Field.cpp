#pragma once
#include "Field.h"
#include "Hand.h"
#include "Card.h"

Field::Field(const std::string& name, const std::string& imagePath,
    Hand* hand) : GameObject()
{
    CreateSlots(hand);
    CreateAvatar(name, imagePath);
}

void Field::SetOpposingField(Field* opposing)
{
    if (opposingField) return;

    opposingField = opposing;
    opposing->SetOpposingField(this);
}

void Field::PlayTurn()
{
    for (int i = 0;i < Conf::MAX_CARDS;i++)
    {
        TriggerCard(i);
    }
}

void Field::TriggerCard(int index)
{
    if (minionPlaced[index]) minionPlaced[index]->Trigger(this, index);
}

void Field::PlaceCard(std::unique_ptr<GameObject> card, int slotIndex)
{
    if (!card || !isEnabled) return;

    Card* cardRef = card->GetComponent<Card>();
    slots[slotIndex]->GetParent()->AdoptChild(std::move(card));
    cardRef->GetParent()->SetRelPosition(0, 0);

    Minion* minion;
    cardRef->TransformToMinion(this, minion);

    minionPlaced[slotIndex] = minion;
    slots[slotIndex]->SetEnabled(false);
}

void Field::SetEnabled(bool enabled) { isEnabled = enabled; }

#pragma region Init
void Field::CreateSlots(Hand* hand)
{
    auto slotsObj = UIFactory::GetLayout<GameObject>(this, new Row(), Conf::MAX_CARDS,
        0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);

    int index = 0;
    for (const auto slot : slotsObj)
    {
        Button* button = new Button(slot);
        Image* image = new Image(slot, Conf::PLACEHOLDER_IMAGE);

        if (hand) AssignHand(hand, button, slot, index);

        slot->AddComponent(button);
        slot->AddComponent(image);
        slots[index] = button;
        index++;
    }

    for (int i = 0; i < Conf::MAX_CARDS; i++)
    {
        minionPlaced[i] = nullptr;
    }
}

void Field::CreateAvatar(const std::string& name, const std::string& imagePath)
{
    int w = Conf::AVATAR_WIDTH;
    int h = Conf::AVATAR_HEIGHT;

    auto avatar = std::make_unique<GameObject>(0, 0, w, h);
    Image* image = new Image(avatar.get(), imagePath);

    avatar->AdoptChild(std::move(UIFactory::NewText(0, 10, w, 30, name)));

    avatar->AdoptChild(std::move(UIFactory::NewText(10, h - 30, w / 5, 20, sanityText)));
    sanityText->SetText(std::to_string(sanity), Conf::SPIRAL_COLOR);

    avatar->AddComponent(image);

    GetComponent<Layout>()->AddGameObject(std::move(avatar));
}

void Field::AssignHand(Hand* hand, Button* button, GameObject* slot, int index)
{
    button->AddOnLeftClick([this, hand, button, slot, index] {
        std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
        PlaceCard(std::move(cardOriginal), index);
        });
}
#pragma endregion
