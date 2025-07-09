#pragma once
#include <unordered_map>
#include <functional>
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "AnimationManager.h"
using namespace Engine;

enum TutorialStep {
	GameStart,
	CardPlace,
	EnemyPlace,
	ShopPick,
	DeckAdd
};

class TutorialManager
{
public:
	TutorialManager() {}

	void Init(GameObject* scene, GameObject* mainScene)
	{
		this->mainScene = mainScene;
		CreateTutorial(scene);

		tutorialProgression = {
			{ GameStart, false },
			{ CardPlace, false },
			{ EnemyPlace, false },
			{ ShopPick, false },
			{ DeckAdd, false }
		};
	}

	void CallTutorial(TutorialStep step)
	{
		if (tutorialProgression[step] || !isTutorialValid) return;

		switch (step)
		{
		case GameStart:
			CallTutorial("You are stuck in this loop. Left-Click me to continue. Or Right click to shut me up until the new loop",
			[this] {CallTutorial("Click card to choose it - Then slot to put it");});
			break;
		case CardPlace:
			CallTutorialDelayed("This card will give you spiral", 200,
			[this] {CallTutorial("To spiral out this day, you need to get 15 of them",
			[this] {CallTutorial("Right Click objects to understand how they work", 
			[this] {CallTutorial("To start your turn you need to start your cycle");});});});
			break;
		case EnemyPlace:
			CallTutorialDelayed("Oh yeah, by the way, world is still spinning around", 200);
			break;
		case ShopPick:
			CallTutorialDelayed("After you loop yourself. You learn new ways to obsess", 200);
			break;
		case DeckAdd:
			CallTutorialDelayed("Your ways to obsess are now in your mind", 200, 
			[this] {CallTutorial("You are on your own now");});
			break;
		default:
			break;
		}

		SoundManager::GetInstance().PlaySFX(Conf::SOUND_TUTORIAL);
		tutorialProgression[step] = true;
	}

private:
	GameObject* tutorial;
	Text* tutorialText;
	Button* tutorialButton;
	bool isTutorialValid = true;

	GameObject* mainScene;
	std::unordered_map<TutorialStep, bool> tutorialProgression;

	void CreateTutorial(GameObject* scene)
	{
		int w = 350;
		int h = 200;

		int xCenter = (Conf::WINDOW_WIDTH - w) / 2;
		int yCenter = (Conf::WINDOW_HEIGHT - h) / 2;

		auto tutorial = std::make_unique<GameObject>(xCenter, yCenter, w, h);
		tutorial->AddComponent(new Image(tutorial.get(), Conf::FRAME_IMAGE));

		tutorial->AdoptChild(std::move(UIFactory::NewText(0, 0, w, h, tutorialText, 20)));
		tutorialText->SetText("Text", { 0,0,0,255 });

		tutorialButton = new Button(tutorial.get());
		tutorialButton->AddOnRightClick([this] {
			isTutorialValid = false;
			HideTutorial();
			});

		tutorialButton->AddOnLeftClick([this] {
			HideTutorial();
			});
		tutorial->AddComponent(tutorialButton);

		this->tutorial = tutorial.get();
		tutorial->SetActive(false);

		scene->AdoptChild(std::move(tutorial));
	}

	void HideTutorial()
	{
		tutorial->SetActive(false);
		mainScene->SetInteractable(true);
	}

	void CallTutorialDelayed(const std::string text, int delay, std::function<void()> onClick = nullptr)
	{
		AnimationManager::GetInstance().PlayDelayedAnimation(
			[this, text, onClick] {CallTutorial(text, onClick);}, delay);
	}

	void CallTutorial(const std::string text, std::function<void()> onClick = nullptr)
	{
		tutorialButton->ClearLeftClick();
		tutorialButton->AddOnLeftClick([this] {HideTutorial();});

		mainScene->SetInteractable(false);
		tutorial->SetActive(true);
		tutorialText->SetText(text);

		if (onClick) tutorialButton->AddOnLeftClick([this, onClick] {onClick();});
	}
};