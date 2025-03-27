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

    void TriggerCard(int index);
    void PlaceCard(std::unique_ptr<GameObject> card, int slotIndex);

    void PlayTurn();

    void ChangeSpiral(int amount);

private:
    Field* opposingField;
    Table* table;

    Minion** minionPlaced = new Minion * [Conf::MAX_CARDS];
    Button** slots = new Button * [Conf::MAX_CARDS];
    int cardQueue = 0;

    void QueueCardAnimation(int index);

    void CreateSlots(Hand* hand);
    void AssignHand(Hand* hand, Button* button, int index);
};
