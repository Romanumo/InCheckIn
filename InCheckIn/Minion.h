#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "Field.h"
using namespace Engine;

struct MinionStats
{
	MinionStats() {}
	MinionStats(const std::string& name, const std::string& desc, 
		std::function<bool(Minion*,int)> trigger)
	{
		this->name = name;
		this->desc = desc;
		if (trigger) this->onTrigger = trigger;
	}

	std::string name;
	std::string desc;
	std::function<bool(Minion*, int)> onTrigger;
};

class Minion : public Component
{
public:
	Minion(GameObject* parentCard, Field* field, MinionStats stats) :
		Component(parentCard), field(field), stats(stats) {}

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