#pragma once
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
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
		CreateWindow(scene);
	}

	void CallHint(int x, int y, const std::string& name, const std::string& desc)
	{
		hint->SetActive(true);
		hint->SetRelPosition(x, y);
		hintName->SetText(name);
		hintDesc->SetText(desc);
	}

	void CallWindow(const std::string& text)
	{
		globalWindow->SetActive(true);
		globalText->SetText(text);
	}

	void HideWindow() { globalWindow->SetActive(false); }
	void HideHint() { hint->SetActive(false); }

private:
	GameObject* hint;
	Text* hintName;
	Text* hintDesc;

	GameObject* globalWindow;
	Text* globalText;

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

	void CreateWindow(GameObject* scene)
	{
		int w = 250;
		int h = 100;

		int xCenter = (Conf::WINDOW_WIDTH - w) / 2;
		int yCenter = (Conf::WINDOW_HEIGHT - h) / 2;

		auto globalWindowObj = std::make_unique<GameObject>(xCenter, yCenter, w, h);
		globalWindowObj->AddComponent(new Image(globalWindowObj.get(), Conf::FRAME_IMAGE));

		globalText = new Text(globalWindowObj.get(), " ");
		globalWindowObj->AddComponent(globalText);

		globalWindow = globalWindowObj.get();
		globalWindow->SetActive(false);

		scene->AdoptChild(std::move(globalWindowObj));
	}
};
