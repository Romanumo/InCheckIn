#pragma once
#include <SDL.h>
#include <memory>

#include "Layout/Row.h"
#include "Layout/Column.h"

#include "Button.h"
#include "Image.h"
#include "Text.h"
#include "TextSurface.h"
#include "Rectangle.h"
#include "GameObject.h"
#include "Component.h"

namespace Engine
{
	static class UIFactory
	{
	public:
		static GameObject* GetButton(int x, int y, int w, int h)
		{
			GameObject* button = new GameObject(x, y, w, h);
			Button* controller = new Button(button);
			Rectangle* visual = new Rectangle(button);
			visual->SetColor(Config::BUTTON_COLOR);

			controller->AddOnHoverEnter([visual] {
				visual->SetColor(Config::BUTTON_HOVER_COLOR);
				});

			controller->AddOnHoverExit([visual] {
				visual->SetColor(Config::BUTTON_COLOR);
				});

			button->AddComponent(controller);
			button->AddComponent(visual);
			return button;
		}

		static GameObject* GetButton(int x, int y, int w, int h, const std::string& text)
		{
			GameObject* button = GetButton(x, y, w, h);
			Text* textComponent = new Text(button, text, {0, 0, 0, 255}, 14);

			button->AddComponent(textComponent);
			return button;
		}

		static void GetRowComponent(GameObject* host, std::vector<std::unique_ptr<GameObject>>&& children)
		{
			Row* rowComponent = new Row(host, Config::PADDING, 0, std::move(children));
			host->AddComponent(rowComponent);
		}

		static void GetColumnComponent(GameObject* host, std::vector<std::unique_ptr<GameObject>>&& children)
		{
			Column* columnComponent = new Column(host, Config::PADDING, 0, std::move(children));
			host->AddComponent(columnComponent);
		}

		static GameObject* GetRow(int x, int y, std::vector<std::unique_ptr<GameObject>>&& children)
		{
			GameObject* row = new GameObject(x, y, 0, 0);
			GetRowComponent(row, std::move(children));
			return row;
		}

		static GameObject* GetColumn(int x, int y, std::vector<std::unique_ptr<GameObject>>&& children)
		{
			GameObject* column = new GameObject(x, y, 0, 0);
			GetColumnComponent(column, std::move(children));
			return column;
		}
	};
}