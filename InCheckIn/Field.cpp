#pragma once
#include "AnimationManager.h"
#include "Table.h"
#include "Field.h"
#include "Hand.h"
#include "Card.h"

Field::Field(Table* table, Hand* hand) : GameObject(), table(table)
{
    CreateSlots(hand);
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
    if (!card) return;

    Card* cardRef = card->GetComponent<Card>();
    slots[slotIndex]->GetParent()->AdoptChild(std::move(card));
    cardRef->GetParent()->SetRelPosition(0, 0);

    Minion* minion;
    cardRef->TransformToMinion(this, minion);

    minionPlaced[slotIndex] = minion;
    slots[slotIndex]->SetEnabled(false);
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

Minion* Field::GetMinionAt(int index) { return minionPlaced[index]; }
void Field::ChangeSpiral(int amount) { table->ChangeSpiral(amount); }
Field* Field::GetOpposingField() { return opposingField; }

#pragma region Init
void Field::CreateSlots(Hand* hand)
{
    auto slotsObj = UIFactory::GetLayout<GameObject>(this, new Row(), Conf::MAX_CARDS,
        0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);

    int index = 0;
    for (const auto slot : slotsObj)
    {
        Button* button = new Button(slot);
        slot->AddComponent(new Image(slot, Conf::SLOT_IMAGE));

        if (hand) AssignHand(hand, button, index);
        slot->AddComponent(button);

        minionPlaced[index] = nullptr;
        slots[index] = button;
        index++;
    }
}

void Field::AssignHand(Hand* hand, Button* button, int index)
{
    Image* slotImage = button->GetParent()->GetComponent<Image>();
    button->AddOnLeftClick([this, hand, index] {
        int cardCost = hand->GetChosenCardSpiral();
        if (table->GetSpiral() < cardCost) return;

        std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
        if (!cardOriginal) return;

        table->ChangeSpiral(cardCost * -1);
        PlaceCard(std::move(cardOriginal), index);
        });

    button->AddOnHoverEnter([slotImage] {
        slotImage->SetImage(Conf::SLOT_SELECTED_IMAGE);
        });

    button->AddOnHoverExit([slotImage] {
        slotImage->SetImage(Conf::SLOT_IMAGE);
        });
}
#pragma endregion
