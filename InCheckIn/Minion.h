#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "Field.h"
using namespace Engine;

class Minion : public Component
{
public:
	Minion(int sanity, int attack, const std::string& name) : Component()
	{
		this->name = name;
		this->attack = attack;
		this->sanity = sanity;
	}

	void Trigger(Field* field, int index)
	{
		if (onTrigger) onTrigger(field, index);
	}

	void SetTrigger(std::function<void(Field*, int)> trigger)
	{
		if (trigger) onTrigger = trigger;
	}

	void SetParent(GameObject* parent) { this->parent = parent; }
	int GetSanity() { return sanity; }
	std::string GetName() { return name; }

	virtual void Render(SDL_Surface* surface) {}
	virtual void HandleEvent(const SDL_Event& event) {}

private:
	int attack;
	int sanity;
	std::string name;
	std::function<void(Field*, int)> onTrigger;
};