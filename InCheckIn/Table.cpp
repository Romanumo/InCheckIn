#pragma once
#include "Table.h"
#include "Deck.h"
#include "Field.h"
#include "Hand.h"

Table::Table() : Engine::GameObject(Conf::PADDING, Conf::PADDING, 0, 0)
{
    deck = std::make_unique<Deck>();
    CreateTable();
    playerField->SetOpposingField(enemyField);

    enemyField->PlaceCard(CardFactory::NewCard(CardFactory::Sun()), 2);
    enemyField->PlaceCard(CardFactory::NewCard(CardFactory::Sun()), 3);
}

void Table::NextTurn()
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

void Table::ChangeSpiralCombo(int spiralChange)
{
    spiralCombo += spiralChange;
    std::string sign = (spiralCombo > 0) ? "+" : "";
    spiralComboText->SetText(sign + std::to_string(spiralCombo));
}

void Table::ApplySpiralCombo()
{
    spiral += spiralCombo;
    spiralText->SetText(std::to_string(spiral) + "/" + std::to_string(neededSpiral));

    spiralCombo = 0;
    spiralComboText->SetText("0");

    if (spiral < 0) Lose();
    else if (spiral >= neededSpiral) Win();
}

void Table::SetHammerMode(bool hammerMode)
{
    isHammer = hammerMode;
    if (isHammer) hammerImage->SetImage(Conf::CARD_IMAGE_PROTOTYPE);
    else hammerImage->SetImage(Conf::HOBBY_IMAGE);
}

int Table::GetSpiral() { return spiral; }
bool Table::GetHammerMode() { return isHammer; }

void Table::Lose()
{
    std::cout << "You lost!" << std::endl;
}

void Table::Win()
{
    std::cout << "You Won!" << std::endl;
}

#pragma region Init
void Table::CreateTable()
{
    auto eFieldObj = std::make_unique<Field>(this);
    enemyField = eFieldObj.get();

    auto handObj = std::make_unique<Hand>();
    hand = handObj.get();

    auto pFieldObj = std::make_unique<Field>(this, handObj.get());
    playerField = pFieldObj.get();

    Engine::Layout* col = new Engine::Layout(this, new Engine::Column(),
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
    AddComponent(col);
}

std::unique_ptr<GameObject> Table::CreateTurnBell()
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

std::unique_ptr<GameObject> Table::CreateHammer()
{
    int w = Conf::AVATAR_WIDTH;
    int h = Conf::AVATAR_HEIGHT;

    auto hammer = std::make_unique<GameObject>(0, 0, w, h);
    hammerImage = new Image(hammer.get(), Conf::HOBBY_IMAGE);
    hammer->AddComponent(hammerImage);

    Button* button = new Button(hammer.get());
    button->AddOnLeftClick([&] {
        SetHammerMode(!isHammer);
        });

    hammer->AddComponent(button);

    return hammer;
}

std::unique_ptr<GameObject> Table::CreateSpiralResource()
{
    int w = Conf::AVATAR_WIDTH;
    int h = Conf::AVATAR_HEIGHT;

    auto spiralIcon = std::make_unique<GameObject>(0, 0, w, h);
    spiralIcon->AddComponent(new Image(spiralIcon.get(), Conf::SPIRAL_IMAGE));

    spiralIcon->AdoptChild(std::move(UIFactory::NewText(w / 2 + 25, h / 2 - 20, w / 3, 40, spiralText)));
    spiralText->SetText(std::to_string(spiral) + "/" + std::to_string(neededSpiral),
        Conf::SPIRAL_COLOR);

    spiralIcon->AdoptChild(std::move(UIFactory::NewText(w / 2 + 10, h / 2 + 10, w / 3, 40, spiralComboText)));
    spiralComboText->SetText("0", Conf::SPIRAL_COLOR);
    return spiralIcon;
}
#pragma endregion