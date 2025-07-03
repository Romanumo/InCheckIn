#pragma once
#include <functional>
#include "Engine/UIFactory.h"
#include "Engine/GameObject.h"
#include "TutorialManager.h"
#include "PopUpManager.h"
#include "GameManager.h"
#include "EnemyAI.h"
#include "Shop.h"
#include "Deck.h"
#include "Field.h"
#include "Spiral.h"
#include "Hammer.h"
#include "Hand.h"
using namespace Engine;

//Creates Scenes & Bind them
class GameUIBuilder
{
public:    
    static void CreateTable(GameObject* scene, std::function<void()> onWin)
    {
        auto spiral = std::make_unique<Spiral>(GM);
        auto hammer = std::make_unique<Hammer>(GM);

        GM.SetMeasureRef(spiral->GetSpiralRef());
        GM.AddOnWin(onWin);
        deck.ConnectToGM(GM);

        auto handObj = std::make_unique<Hand>(GM, deck);
        Hand* hand = handObj.get();

        FieldContext enemy = FieldContext(&GM, nullptr, spiral.get());
        auto eFieldObj = std::make_unique<Field>(enemy);

        FieldContext player = FieldContext(&GM, hammer.get(), spiral.get());
        auto pFieldObj = std::make_unique<Field>(player, 1, handObj.get());

        EnemyAI* AI = new EnemyAI(GM, eFieldObj.get());
        BindTutorial(pFieldObj.get(), eFieldObj.get());

        Layout* col = new Layout(scene, new Column(), Conf::PADDING, 0);
        col->AddGameObject(std::move(
            UIFactory::NewRow(
                std::move(eFieldObj),
                std::move(hammer))));
        col->AddGameObject(std::move(
            UIFactory::NewRow(
                std::move(pFieldObj),
                std::move(spiral))));
        col->AddGameObject(std::move(
            UIFactory::NewRow(
                std::move(handObj),
                std::move(CreateTurnBell(hand)))));

        scene->AddComponent(col);

        GM.NewGame();
    }

    static void CreateShop(GameObject* scene, std::function<void()> onPickUp)
    {
        auto title = UIFactory::NewText(0, 0, (Conf::PADDING + Conf::CARD_WIDTH) * 3, 100,
            "Choose A Card", { 0,0,0, 255 }, 25);

        auto shopObj = std::make_unique<Shop>(&deck, onPickUp);

        Layout* col = new Layout(scene, new Column(), Conf::PADDING, 0);
        col->AddGameObject(std::move(title));
        col->AddGameObject(std::move(shopObj));
        scene->AddComponent(col);
    }

    static void CreateUI(GameObject* scene, GameObject* gameScene)
    {
        TM.Init(scene, gameScene);
        PopUpManager::GetInstance().Init(scene);

        TM.CallTutorial(TutorialStep::GameStart);
    }

private:
    inline static GameManager GM = GameManager();
    inline static TutorialManager TM = TutorialManager();
    inline static Deck deck = Deck();

    static void BindTutorial(Field* player, Field* enemy)
    {
        GM.AddOnNewGame([tuts = &TM](int level) {
            if(level == 1) tuts->CallTutorial(TutorialStep::ShopPick);
        });

        player->AddOnCardPlaced([tuts = &TM] {
            tuts->CallTutorial(TutorialStep::CardPlace);
        });

        enemy->AddOnCardPlaced([tuts = &TM] {
            tuts->CallTutorial(TutorialStep::EnemyPlace);
        });

        deck.AddOnNewCard([tuts = &TM] {
            tuts->CallTutorial(TutorialStep::DeckAdd);
        });
    }

    static std::unique_ptr<GameObject> CreateTurnBell(Hand* hand)
    {
        auto bellObj = std::make_unique<GameObject>(0, 0,
            Conf::TURNBT_WIDTH, Conf::TURNBT_HEIGHT);
        bellObj->AddComponent(new Image(bellObj.get(), Conf::TURNBT_IMAGE));

        Button* button = new Button(bellObj.get());

        button->AddOnLeftClick([&, hand] {
            if (GM.GetState() == GameFlow::CHOOSING)
            {
                GM.NextTurn();
                hand->AddCard(CardFactory::NewCard(deck.GetCard()));
				SoundManager::GetInstance().PlaySFX(Conf::SOUND_BELL);
            }
            });

        bellObj->AddComponent(button);
        return bellObj;
    }
};