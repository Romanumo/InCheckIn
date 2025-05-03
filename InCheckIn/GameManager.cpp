#pragma once
#include "GameManager.h"
#include "HintManager.h"

void GameManager::Init()
{
    scene = std::make_unique<GameObject>(Conf::PADDING, Conf::PADDING, 0, 0);

    CreateTable();
    deck = std::make_unique<Deck>();
    playerField->SetOpposingField(enemyField);
    HintManager::GetInstance().Init(scene.get());

    enemyField->PlaceCard(CardFactory::NewCard(CardFactory::Sun()), 2);
    enemyField->PlaceCard(CardFactory::NewCard(CardFactory::Sun()), 3);
}

void GameManager::NextTurn()
{
    switch (turn)
    {
    case CHOOSING:
        std::cout << "Chosing Ends" << std::endl;
        turn = GameFlow::PLAYER;
        hand->SetEnabled(false);
        isHammer = false;
        playerField->PlayTurn();
        break;
    case PLAYER:
        std::cout << "Player Turn Ends" << std::endl;
        turn = GameFlow::ENEMY;
        enemyField->PlayTurn();
        break;
    case ENEMY:
        std::cout << "Enemy Turn Ends" << std::endl;
        turn = GameFlow::CHOOSING;
        hand->SetEnabled(true);
        break;
    }
}

void GameManager::ChangeSpiralCombo(int spiralChange)
{
    spiralCombo += spiralChange;
    std::string sign = (spiralCombo > 0) ? "+" : "";
    spiralComboText->SetText(sign + std::to_string(spiralCombo));
}

void GameManager::ApplySpiralCombo()
{
    spiral += spiralCombo;
    spiralText->SetText(std::to_string(spiral) + "/" + std::to_string(neededSpiral));

    spiralCombo = 0;
    spiralComboText->SetText("0");

    if (spiral < 0) Lose();
    else if (spiral >= neededSpiral) Win();
}

void GameManager::SetHammerMode(bool hammerMode)
{
    isHammer = hammerMode;
    if (isHammer) hammerImage->SetImage(Conf::HAMMER_ON_IMAGE);
    else hammerImage->SetImage(Conf::HAMMER_OFF_IMAGE);
}

int GameManager::GetSpiral() { return spiral; }
bool GameManager::GetHammerMode() { return isHammer; }
const GameObject* GameManager::GetScene() { return scene.get(); }

void GameManager::Lose()
{
    std::cout << "You lost!" << std::endl;
}

void GameManager::Win()
{
    std::cout << "You Won!" << std::endl;
}


#pragma region Init
void GameManager::CreateTable()
{
    auto eFieldObj = std::make_unique<Field>();
    enemyField = eFieldObj.get();

    auto handObj = std::make_unique<Hand>();
    hand = handObj.get();

    auto pFieldObj = std::make_unique<Field>(1, handObj.get());
    playerField = pFieldObj.get();

    Engine::Layout* col = new Engine::Layout(scene.get(), new Engine::Column(),
        Conf::PADDING, 0);
    col->AddGameObject(std::move(
        UIFactory::NewRow(
            std::move(eFieldObj),
            std::move(CreateHammer()))));
    col->AddGameObject(std::move(
        UIFactory::NewRow(
            std::move(pFieldObj),
            std::move(CreateSpiralResource()))));
    col->AddGameObject(std::move(
        UIFactory::NewRow(
            std::move(handObj),
            std::move(CreateTurnBell()))));
    scene->AddComponent(col);
}

std::unique_ptr<GameObject> GameManager::CreateTurnBell()
{
    auto bellObj = std::make_unique<GameObject>(0, 0,
        Conf::TURNBT_WIDTH, Conf::TURNBT_HEIGHT);
    bellObj->AddComponent(new Image(bellObj.get(), Conf::TURNBT_IMAGE));

    Button* button = new Button(bellObj.get());

    button->AddOnLeftClick([&] {
        if (turn == GameFlow::CHOOSING)
        {
            NextTurn();
            hand->AddCard(CardFactory::NewCard(deck->GetCard()));
        }
        });

    bellObj->AddComponent(button);
    return bellObj;
}

std::unique_ptr<GameObject> GameManager::CreateHammer()
{
    int w = Conf::AVATAR_WIDTH;
    int h = Conf::AVATAR_HEIGHT;

    auto hammer = std::make_unique<GameObject>(0, 0, w, h);
    hammerImage = new Image(hammer.get(), Conf::HAMMER_OFF_IMAGE);
    hammer->AddComponent(hammerImage);

    Button* button = new Button(hammer.get());
    button->AddOnLeftClick([&] {
        SetHammerMode(!isHammer);
        });

    hammer->AddComponent(button);

    return hammer;
}

std::unique_ptr<GameObject> GameManager::CreateSpiralResource()
{
    int w = Conf::AVATAR_WIDTH;
    int h = Conf::AVATAR_HEIGHT;

    auto spiralIcon = std::make_unique<GameObject>(0, 0, w, h);
    spiralIcon->AddComponent(new Image(spiralIcon.get(), Conf::SPIRAL_IMAGE));

    spiralIcon->AdoptChild(std::move(UIFactory::NewText(w / 2, h / 2 - 20, w / 2, 40, spiralText)));
    spiralText->SetText(std::to_string(spiral) + "/" + std::to_string(neededSpiral),
        Conf::SPIRAL_COLOR);

    spiralIcon->AdoptChild(std::move(UIFactory::NewText(w / 2, h / 2 + 10, w / 2, 40, spiralComboText)));
    spiralComboText->SetText("0", Conf::SPIRAL_COLOR);
    return spiralIcon;
}

GameFlow GameManager::turn = GameFlow::CHOOSING;
Field* GameManager::enemyField = nullptr;
Field* GameManager::playerField = nullptr;
Hand* GameManager::hand = nullptr;
std::unique_ptr<GameObject> GameManager::scene = nullptr;
std::unique_ptr<Deck> GameManager::deck = nullptr;

bool GameManager::isHammer = false;
Image* GameManager::hammerImage = nullptr;

Text* GameManager::spiralText = nullptr;
Text* GameManager::spiralComboText = nullptr;
int GameManager::spiral = 10;
int GameManager::spiralCombo = 0;
int GameManager::neededSpiral = 30;
#pragma endregion