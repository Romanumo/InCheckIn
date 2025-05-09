#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "Field.h"
using namespace Engine;

class Minion;

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

class Minion : public Button
{
public:
	Minion(GameObject* parentCard, Field* field, MinionStats stats, Hammer* hammer = nullptr) :
		Button(parentCard), field(field), stats(stats) 
	{
		AddOnRightClick([this] {
			const SDL_Rect* absTF = GetParent()->GetAbsTf();
			HintManager::GetInstance().CallHint(absTF->x + absTF->w, absTF->y,
				GetName(), GetDesc());
			});

		AddOnHoverExit([=] {
			HintManager::GetInstance().HideHint();
			});

		AddHammerEvents(hammer);
	}

	//True - Continue Card continuation; False - Wait
	bool Trigger(int index)
	{
		if (stats.onTrigger) return stats.onTrigger(this, index);
		return true;
	}

	std::string GetName() { return stats.name; }
	std::string GetDesc() { return stats.desc; }
	void ChangeDesc(const std::string& desc) { stats.desc = desc; }
	Field* GetField() { return field; }

private:
	MinionStats stats;
	Field* field;

	void AddHammerEvents(Hammer* hammer)
	{
		if (!hammer) return;

		AddOnLeftClick([&, hammer] {
			if (hammer->GetHammerMode())
			{
				hammer->SetHammerMode(false);
				field->RemoveCard(this);
			}
			});
	}
};