#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "Field.h"
using namespace Engine;

//Minion in a way is both a capsule, a container holding a minion relavant info
//But can be activated later to be an actual component, when played in board

class Minion : public Component
{
public:
	Minion(const std::string& name) : Component()
	{
		this->name = name;
	}

	void ActivateMinion(GameObject* parent, Field* field)
	{
		this->parent = parent;
		this->field = field;
		parent->AddComponent(this);
	}

	void Trigger(Field* field, int index)
	{
		if (onTrigger) onTrigger(field, index);
	}

	void SetTrigger(std::function<void(Field*, int)> trigger)
	{
		if (trigger) onTrigger = trigger;
	}

	std::string GetName() { return name; }

	virtual void Render(SDL_Surface* surface) {}
	virtual void HandleEvent(const SDL_Event& event) {}

private:
	std::string name;
	std::function<void(Field*, int)> onTrigger;

	Field* field;
};