#pragma once
#include "AnimationManager.h"
#include "Table.h"
#include "Field.h"
#include "Hand.h"
#include "Card.h"

Field::Field(const std::string& name, const std::string& imagePath, Table* table,
    Hand* hand, int initSpiral) : GameObject(), table(table)
{
    spiral = initSpiral;
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
    TriggerCard(0);
    AnimationManager::GetInstance().PlayNextAnimation();
}

void Field::TriggerCard(int index)
{
    if (index >= Conf::MAX_CARDS)
    {
        if (cardQueue >= Conf::MAX_CARDS)
        {
            cardQueue = 0;
            table->NextTurn();
        }
        return;
    }

    if (!minionPlaced[index]) TriggerCard(++cardQueue);
    else QueueCardAnimation(index);
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

bool Field::SpendSpiral(int spiralCost)
{
    if (spiralCost > spiral) return false;
    spiral -= spiralCost;
    sanityText->SetText(std::to_string(spiral));
    return true;
}

void Field::QueueCardAnimation(int index)
{
    AnimationManager::GetInstance().EnqueueAnimation(Animation(
        [=] {
        const SDL_Rect* rect = minionPlaced[index]->GetParent()->GetRelTf();
        minionPlaced[index]->GetParent()->SetRelPosition(rect->x, rect->y - 15);

        bool isBreaker = minionPlaced[index]->Trigger(index);
        if (isBreaker) TriggerCard(++cardQueue);

        }, [=] {
            const SDL_Rect* rect = minionPlaced[index]->GetParent()->GetRelTf();
            minionPlaced[index]->GetParent()->SetRelPosition(rect->x, rect->y + 15);
        }, 500));
}

Field* Field::GetOpposingField() { return opposingField; }

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
        slot->AddComponent(new Image(slot, Conf::PLACEHOLDER_IMAGE));

        if (hand) AssignHand(hand, button, index);
        slot->AddComponent(button);

        minionPlaced[index] = nullptr;
        slots[index] = button;
        index++;
    }
}

void Field::CreateAvatar(const std::string& name, const std::string& imagePath)
{
    int w = Conf::AVATAR_WIDTH;
    int h = Conf::AVATAR_HEIGHT;

    auto avatar = std::make_unique<GameObject>(0, 0, w, h);
    avatar->AddComponent(new Image(avatar.get(), imagePath));
    avatar->AdoptChild(std::move(UIFactory::NewText(0, 10, w, 30, name)));
    avatar->AdoptChild(std::move(UIFactory::NewText(10, h - 30, w / 5, 20, sanityText)));
    sanityText->SetText(std::to_string(spiral), Conf::SPIRAL_COLOR);

    GetComponent<Layout>()->AddGameObject(std::move(avatar));
}

void Field::AssignHand(Hand* hand, Button* button, int index)
{
    button->AddOnLeftClick([this, hand, index] {
        if (!SpendSpiral(hand->GetChosenCardSpiral())) return;

        std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
        PlaceCard(std::move(cardOriginal), index);
        });
}
#pragma endregion
