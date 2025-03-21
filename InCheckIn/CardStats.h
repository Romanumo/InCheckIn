#pragma once
#include <functional>
#include "Engine/Button.h"
#include "Engine/GameObject.h"

class Card;

struct CardStats
{
public:
	CardStats(const std::string& name, const std::string& imagePath,
		int attack, int sanity, std::function<void(Card*)> trigger)
	{
		this->name = name;
		this->imagePath = imagePath;
		this->attack = attack;
		this->sanity = sanity;
		if (trigger) onTrigger = trigger;
	}

	std::string name;
	std::string imagePath;
	int attack;
	int sanity;
	std::function<void(Card*)> onTrigger;
};