#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "Field.h"
using namespace Engine;

struct MinionStats
{
	MinionStats() {}
	MinionStats(const std::string& name, std::function<bool(Minion*,int)> trigger)
	{
		this->name = name;
		if (trigger) this->onTrigger = trigger;
	}

	std::string name;
	std::function<bool(Minion*, int)> onTrigger;
};

class Minion : public Component
{
public:
	Minion(GameObject* parent, Field* field, MinionStats stats) :
		Component(parent), field(field), stats(stats) {}

	//True - Continue Card continuation; False - Wait
	bool Trigger(int index)
	{
		if (stats.onTrigger) return stats.onTrigger(this, index);
		return true;
	}

	std::string GetName() { return stats.name; }
	Field* GetField() { return field; }

	virtual void Render(SDL_Surface* surface) {}
	virtual void HandleEvent(const SDL_Event& event) {}

private:
	MinionStats stats;
	Field* field;
};