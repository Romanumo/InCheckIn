#pragma once
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
using namespace Engine;

class HintManager
{
public:
	static void Init(GameObject *scene)
	{
		if (hintWindow) return;

		auto hint = std::make_unique<GameObject>(0, 0, 300, 300);
		hint->AddComponent(new Image(hint.get(), Conf::CARD_IMAGE_PROTOTYPE));

		hintWindow = hint.get();
		scene->AdoptChild(std::move(hint));
	}

	static void ShowHint(int x, int y)
	{

	}

private:
	HintManager() {}
	~HintManager() {}

	static GameObject* hintWindow;
};