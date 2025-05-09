#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "GameManager.h"
#include "Spiral.h"
#include "Hammer.h"
using namespace Engine;

class Hand;
class Minion;

class FieldContext
{
public:
    FieldContext(GameManager* GM, Hammer* hammer, Spiral* spiral)
    {
        this->GM = GM;
        this->hammer = hammer;
        this->spiral = spiral;
    }

    GameManager* GM;
    Hammer* hammer;
    Spiral* spiral;
};

class Field : public GameObject
{
public:
    Field(FieldContext context, int comboAdd = 0, Hand* hand = nullptr);

    Minion* GetMinionAt(int index);
    void ChangeSpiralCombo(int amount);
    bool IsFull();

    void PlayTurn();
    void TriggerCard(int index);
    void RemoveCard(Minion* minion);
    void PlaceCard(std::unique_ptr<GameObject> card, int slotIndex);

private:
    Minion** minionPlaced;
    Button** slots = new Button * [Conf::MAX_CARDS];
    GameObject* queueIndicator;

    FieldContext context;

    int cardQueue = 0;
    int comboAdd = 0;

    void UpdateIndicator();
    void ClearInactive();
    void QueueCardAnimation(int index);

    void CreateSlots(Hand* hand);
    void CreateIndicator();
    void AssignHand(Hand* hand, Button* button, int index);
};
