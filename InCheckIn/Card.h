#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "CardStats.h"
using namespace Engine;

enum CardState
{
	IDLE,
	HOVERED,
	CHOSEN,
	PLAYED
};

//Might need to have 2 states rather. Just make this card into 2 compoennts
//2 have the CardStats passing to eahc other. As I might have a hand return

//BUt Card(Hand) need to have a refence to Card (Minion) either as pointer or a function pointer

class Card : public GameObject
{
public:
	Card(CardStats stats);

	void IntoPlayedState();
	void Deselect();
	void Trigger(Field* field, int index);

	CardState GetState();
	std::string GetName();

private:
	Button* button;
	Text* sanityText;

	CardState state = CardState::IDLE;
	int initY = 0;

	CardStats stats;

	void AssignButton(Engine::Button* button);
};