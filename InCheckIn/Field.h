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
    Field(const std::string& name, const std::string& imagePath, Table* table, 
        Hand* hand = nullptr, int initSpiral = 0);

    void SetOpposingField(Field* opposing);
    Field* GetOpposingField();

    void TriggerCard(int index);
    void PlaceCard(std::unique_ptr<GameObject> card, int slotIndex);

    void PlayTurn();
    bool SpendSpiral(int spiralCost);
    void SetEnabled(bool enabled);

private:
    Text* sanityText;
    int spiral;

    int cardQueue = 0;

    Field* opposingField;
    bool isEnabled = true;
    Table* table;

    Minion** minionPlaced = new Minion * [Conf::MAX_CARDS];
    Button** slots = new Button * [Conf::MAX_CARDS];

    void QueueCardAnimation(int index);

    void CreateSlots(Hand* hand);
    void CreateAvatar(const std::string& name, const std::string& imagePath);
    void AssignHand(Hand* hand, Button* button, int index);
};
