#pragma once
#include "AnimationManager.h"
#include "GameManager.h"
#include "Field.h"
#include "Hand.h"
#include "Card.h"

Field::Field(int comboAdd, Hand* hand) : GameObject(), comboAdd(comboAdd)
{
    CreateSlots(hand);
    CreateIndicator();
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

//Check if you triggered card exists, otherwise it can break order
void Field::TriggerCard(int index)
{
    if (index >= Conf::MAX_CARDS)
    {
        if (cardQueue >= Conf::MAX_CARDS)
        {
            AnimationManager::GetInstance().EnqueueAnimation(Animation([=] {
                cardQueue = 0;
                UpdateIndicator();
                GameManager::ApplySpiralCombo();
                GameManager::NextTurn();
                }, nullptr, 500));
        }
        return;
    }

    UpdateIndicator();
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

    ConnectToField(slotIndex, minion);

    minionPlaced[slotIndex] = minion;
    slots[slotIndex]->SetEnabled(false);
}

void Field::RemoveCard(int index)
{
    for (int i = 0;i<Conf::MAX_CARDS;i++)
    {
        if (minionPlaced[i] && index == i)
        {
            slots[i]->SetEnabled(true);
            slots[i]->GetParent()->RemoveChild(minionPlaced[i]->GetParent());
            minionPlaced[i] = nullptr;
            return;
        }
    }
}

void Field::QueueCardAnimation(int index)
{
    AnimationManager::GetInstance().EnqueueAnimation(Animation(
        [=] {
        const SDL_Rect* rect = minionPlaced[index]->GetParent()->GetRelTf();
        minionPlaced[index]->GetParent()->SetRelPosition(rect->x, rect->y - 15);

        GameManager::ChangeSpiralCombo(comboAdd);
        bool isBreaker = minionPlaced[index]->Trigger(index);
        if (isBreaker)
        {
            AnimationManager::GetInstance().EnqueueAnimation(Animation(
                [=] {
                    TriggerCard(++cardQueue);
                }, [] {}, Conf::NEW_CARD_T));
        }

        }, [=] {
            const SDL_Rect* rect = minionPlaced[index]->GetParent()->GetRelTf();
            minionPlaced[index]->GetParent()->SetRelPosition(rect->x, rect->y + 15);
        }, Conf::CARD_ANIM_T));

    AnimationManager::GetInstance().EnqueuePause(Conf::PAUSE_TIME);
}

void Field::ConnectToField(int index, Minion* minion)
{
    if (!isPlayer) return;

    Button* button = new Button(minion->GetParent());
    button->AddOnLeftClick([&, index] {
        if (GameManager::GetHammerMode())
        {
            GameManager::SetHammerMode(false);
            RemoveCard(index);
        }
        });
    minion->GetParent()->AddComponent(button);
}

void Field::UpdateIndicator()
{
    const SDL_Rect* slotRect = slots[cardQueue]->GetParent()->GetRelTf();
    queueIndicator->SetRelPosition(slotRect->x, slotRect->y);
}

Minion* Field::GetMinionAt(int index) { return minionPlaced[index]; }
void Field::ChangeSpiralCombo(int amount) { GameManager::ChangeSpiralCombo(amount); }
Field* Field::GetOpposingField() { return opposingField; }

#pragma region Init
void Field::CreateSlots(Hand* hand)
{
    minionPlaced = new Minion * [Conf::MAX_CARDS](); 
    auto slotsObj = UIFactory::GetLayout<GameObject>(this, new Row(), Conf::MAX_CARDS,
        0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);

    int index = 0;
    for (const auto slot : slotsObj)
    {
        Button* button = new Button(slot);
        slot->AddComponent(new Image(slot, Conf::SLOT_IMAGE));

        if (hand) AssignHand(hand, button, index);
        slot->AddComponent(button);

        slots[index] = button;
        index++;
    }
}

void Field::CreateIndicator()
{
    auto indicator = std::make_unique<GameObject>(0, 0, Conf::CARD_WIDTH, Conf::CARD_HEIGHT);
    indicator->AddComponent(new Image(indicator.get(), Conf::CARD_IMAGE_INDICATOR));

    queueIndicator = indicator.get();
    this->AdoptChild(std::move(indicator));

    UpdateIndicator();
}

void Field::AssignHand(Hand* hand, Button* button, int index)
{
    Image* slotImage = button->GetParent()->GetComponent<Image>();
    button->AddOnLeftClick([this, hand, index] {
        int cardCost = hand->GetChosenCardSpiral();
        if (GameManager::GetSpiral() < cardCost) return;

        std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
        if (!cardOriginal) return;

        GameManager::ChangeSpiralCombo(cardCost * -1);
        PlaceCard(std::move(cardOriginal), index);
        });

    button->AddOnHoverEnter([slotImage] {
        slotImage->SetImage(Conf::SLOT_SELECTED_IMAGE);
        });

    button->AddOnHoverExit([slotImage] {
        slotImage->SetImage(Conf::SLOT_IMAGE);
        });

    isPlayer = true;
}
#pragma endregion
