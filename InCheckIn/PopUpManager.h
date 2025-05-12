#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "TutorialManager.h"
using namespace Engine;

class PopUpManager
{
public:
	static PopUpManager& GetInstance()
	{
		static PopUpManager popMan;
		return popMan;
	}

	void Init(GameObject* scene)
	{
		CreateHint(scene);
	}

	void CallHint(int x, int y, const std::string& name, const std::string& desc)
	{
		hint->SetActive(true);
		hint->SetRelPosition(x, y);
		hintName->SetText(name);
		hintDesc->SetText(desc);
	}

	void HideHint() { hint->SetActive(false); }

private:
	GameObject* hint;
	Text* hintName;
	Text* hintDesc;

	void CreateHint(GameObject* scene)
	{
		int w = 300;
		int h = 100;

		auto hintWindow = std::make_unique<GameObject>(0, 0, w, h);
		hintWindow->AddComponent(new Image(hintWindow.get(), Conf::FRAME_IMAGE));

		hintWindow->AdoptChild(std::move(UIFactory::NewText(0, 0, 300, h / 2, hintName, 20)));
		hintName->SetText("Spiral", { 0,0,0,255 });

		hintWindow->AdoptChild(std::move(UIFactory::NewText(0, h / 3 + 10, 300, h / 2, hintDesc, 14)));
		hintDesc->SetText("DEsc", { 0,0,0,255 });

		hint = hintWindow.get();
		hint->SetActive(false);

		scene->AdoptChild(std::move(hintWindow));
	}
};
