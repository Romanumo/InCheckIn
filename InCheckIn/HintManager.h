#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
using namespace Engine;

class HintManager
{
public:
	static HintManager& GetInstance()
	{
		static HintManager hintMan;
		return hintMan;
	}

	void Init(GameObject* scene)
	{
		int w = 300;
		int h = 100;

		auto hintWindow = std::make_unique<GameObject>(0, 0, w, h);
		hintWindow->AddComponent(new Image(hintWindow.get(), Conf::FRAME_IMAGE));

		hintWindow->AdoptChild(std::move(UIFactory::NewText(0, 0, 300, h/2, textName, 20)));
		textName->SetText("Spiral", {0,0,0,255});

		hintWindow->AdoptChild(std::move(UIFactory::NewText(0, h/3 + 10, 300, h /2, textDesc, 14)));
		textDesc->SetText("DEsc", { 0,0,0,255 });

		hint = hintWindow.get();
		hint->SetActive(false);

		scene->AdoptChild(std::move(hintWindow));
	}

	void CallHint(int x, int y, const std::string& name, const std::string& desc)
	{
		hint->SetActive(true);
		hint->SetRelPosition(x, y);
		textName->SetText(name);
		textDesc->SetText(desc);
	}

	void HideHint()
	{
		hint->SetActive(false);
	}

private:
	GameObject* hint;
	Text* textName;
	Text* textDesc;
};
