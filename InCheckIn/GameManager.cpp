//#pragma once
//#include "GameManager.h"
//#include "HintManager.h"
//
//void GameManager::Init()
//{
//    CreateShop();
//}
//
//void GameManager::SwitchToGame()
//{
//    mainScene = gameScene.get();
//    deck.Shuffle();
//
//    hand->AddCard(CardFactory::NewCard(deck.GetCard()));
//    hand->AddCard(CardFactory::NewCard(deck.GetCard()));
//    hand->AddCard(CardFactory::NewCard(deck.GetCard()));
//    hand->AddCard(CardFactory::NewCard(deck.GetCard()));
//}
//
//void GameManager::CreateShop()
//{
    //auto title = UIFactory::NewText(0,0, (Conf::PADDING + Conf::CARD_WIDTH) * 3, 100,
    //    "Choose A Card", {0,0,0, 255}, 25);

    //auto shopObj = std::make_unique<Shop>();
    //shop = shopObj.get();

    //Layout* col = new Layout(shopScene.get(), new Engine::Column(),Conf::PADDING, 0);
    //col->AddGameObject(std::move(title));
    //col->AddGameObject(std::move(shopObj));//SHop
    //shopScene->AddComponent(col);
//}