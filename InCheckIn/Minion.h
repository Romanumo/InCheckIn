#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "Field.h"
using namespace Engine;

struct MinionStats
{
	MinionStats(const std::string& name, std::function<void(Minion*, Field*, int)> trigger)
	{
		this->name = name;
		if (trigger) this->onTrigger = trigger;
	}

	std::string name;
	std::function<void(Minion*, Field*, int)> onTrigger;
};

class Minion : public Component
{
public:
	Minion(GameObject* parent, Field* field, MinionStats stats) :
		Component(parent), field(field), stats(stats) { }


	void Trigger(Field* field, int index)
	{
		if (stats.onTrigger) stats.onTrigger(this, field, index);
	}

	std::string GetName() { return stats.name; }

	virtual void Render(SDL_Surface* surface) {}
	virtual void HandleEvent(const SDL_Event& event) {}

private:
	MinionStats stats;
	Field* field;
};