#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
using namespace Engine;

class Hand;
class Minion;
class GameManager;

class Field : public GameObject
{
public:
    Field(int comboAdd = 0, Hand* hand = nullptr);

    void SetOpposingField(Field* opposing);
    Field* GetOpposingField();

    Minion* GetMinionAt(int index);
    void ChangeSpiralCombo(int amount);
    bool IsFull();

    void PlayTurn();
    void TriggerCard(int index);
    void RemoveCard(int index);
    void PlaceCard(std::unique_ptr<GameObject> card, int slotIndex);

private:
    Field* opposingField;

    Minion** minionPlaced;
    Button** slots = new Button * [Conf::MAX_CARDS];
    GameObject* queueIndicator;
    int cardQueue = 0;
    int comboAdd = 0;
    bool isPlayer = false;

    void QueueCardAnimation(int index);
    void UpdateIndicator();
    void ClearInactive();
    void ConnectToField(int index, Minion* minion);

    void CreateSlots(Hand* hand);
    void CreateIndicator();
    void AssignHand(Hand* hand, Button* button, int index);
};
