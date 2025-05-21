#pragma once
#include "Field.h"

#include "AnimationManager.h"
#include "Hand.h"
#include "Card.h"

Field::Field(FieldContext context, int comboAdd, Hand* hand) :
    GameObject(), comboAdd(comboAdd), context(context)
{
    CreateSlots(hand);
    CreateIndicator();

    context.GM->AddOnTurnChange([this, hand](GameFlow turn) {
        if (hand && turn == GameFlow::PLAYER) PlayTurn();
        else if (!hand && turn == GameFlow::ENEMY) PlayTurn();
        });

    context.GM->AddOnNewGame([this] {
        EmptyField();
        });

    onCardPlaced.AddEvent([this] {
        SoundManager::GetInstance().PlaySFX(Conf::SOUND_CARD_PLACE);
    });
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
            AnimationManager::GetInstance().EnqueueAnimation(Animation([&] {
                cardQueue = 0;
                UpdateIndicator();
                ClearInactive();
                context.GM->NextTurn();
                }, nullptr, 500));
        }
        return;
    }

    UpdateIndicator();

    if (!minionPlaced[index] || !minionPlaced[index]->GetParent()->IsActive()) 
        TriggerCard(++cardQueue);
    else QueueCardAnimation(index);
}

void Field::PlaceCard(std::unique_ptr<GameObject> card, int slotIndex)
{
    if (!card) return;

    Card* cardRef = card->GetComponent<Card>();
    slots[slotIndex]->GetParent()->AdoptChild(std::move(card));
    cardRef->GetParent()->SetRelPosition(0, 0);

    Minion* minion;
    cardRef->TransformToMinion(this, minion, context.hammer);

    minionPlaced[slotIndex] = minion;
    slots[slotIndex]->SetEnabled(false);

    onCardPlaced.Invoke();
}

void Field::RemoveCard(Minion* minion)
{
    for (int i = 0;i < Conf::MAX_CARDS;i++)
    {
        if (minionPlaced[i] == minion) RemoveCard(i);
    }
}

void Field::RemoveCard(int index)
{
    if (minionPlaced[index])
    {
        slots[index]->SetEnabled(true);
        slots[index]->GetParent()->RemoveChild(minionPlaced[index]->GetParent());
        minionPlaced[index] = nullptr;
        return;
    }
}

void Field::QueueCardAnimation(int index)
{
    AnimationManager::GetInstance().EnqueueAnimation(Animation(
        [=] {
        const SDL_Rect* rect = minionPlaced[index]->GetParent()->GetRelTf();
        minionPlaced[index]->GetParent()->SetRelPosition(rect->x, rect->y - 15);

        context.spiral->ChangeSpiralCombo(comboAdd);
        SoundManager::GetInstance().PlaySFX(Conf::SOUND_CARD_TRIGGER);
 
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

void Field::UpdateIndicator()
{
    const SDL_Rect* slotRect = slots[cardQueue]->GetParent()->GetRelTf();
    queueIndicator->SetRelPosition(slotRect->x, slotRect->y);
}

void Field::ClearInactive()
{
    for (int i = 0;i < Conf::MAX_CARDS;i++)
    {
        if (minionPlaced[i] && !minionPlaced[i]->GetParent()->IsActive()) RemoveCard(i);
    }
}

void Field::EmptyField()
{
    for (int i = 0;i < Conf::MAX_CARDS;i++)
    {
        if (minionPlaced[i]) RemoveCard(i);
    }
}

bool Field::IsFull()
{
    for (int i = 0;i < Conf::MAX_CARDS;i++)
    {
        if (!minionPlaced[i]) return false;
    }

    return true;
}

void Field::AddOnCardPlaced(std::function<void()> event) { onCardPlaced.AddEvent(event); }
Minion* Field::GetMinionAt(int index) { return minionPlaced[index]; }
void Field::ChangeSpiralCombo(int amount) { context.spiral->ChangeSpiralCombo(amount); }

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
        if (context.spiral->GetSpiral() < cardCost)
        {
            const SDL_Rect* absTF = this->GetAbsTf();
            PopUpManager::GetInstance().CallHint(absTF->x, absTF->y, "Not enough Spiral", " ");
            return;
        }

        std::unique_ptr<GameObject> cardOriginal = hand->PlaceCard();
        if (!cardOriginal) return;

        context.spiral->ChangeSpiralCombo(cardCost * -1);
        PlaceCard(std::move(cardOriginal), index);
        });

    button->AddOnHoverEnter([slotImage] {
        slotImage->SetImage(Conf::SLOT_SELECTED_IMAGE);
        });

    button->AddOnHoverExit([slotImage] {
        slotImage->SetImage(Conf::SLOT_IMAGE);
        PopUpManager::GetInstance().HideHint();
        });
}
#pragma endregion
