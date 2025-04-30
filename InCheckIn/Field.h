#pragma once
#include "Engine/Managers/Globals.h"
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
using namespace Engine;

class Hand;
class Minion;
class Table;

class Field : public GameObject
{
public:
    Field(Table* table, Hand* hand = nullptr);

    void SetOpposingField(Field* opposing);
    Field* GetOpposingField();

    Minion* GetMinionAt(int index);
    void ChangeSpiralCombo(int amount);

    void PlayTurn();
    void TriggerCard(int index);
    void RemoveCard(int index);
    void PlaceCard(std::unique_ptr<GameObject> card, int slotIndex);

private:
    Field* opposingField;
    Table* table;

    Minion** minionPlaced;
    Button** slots = new Button * [Conf::MAX_CARDS];
    GameObject* queueIndicator;
    int cardQueue = 0;
    bool isPlayer = false;

    void QueueCardAnimation(int index);
    void UpdateIndicator();
    void ConnectToField(int index, Minion* minion);

    void CreateSlots(Hand* hand);
    void CreateIndicator();
    void AssignHand(Hand* hand, Button* button, int index);
};
