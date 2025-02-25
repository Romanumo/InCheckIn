#pragma once
#include <SDL.h>

#include "Layout/Row.h"
#include "Layout/Column.h"

#include "Button.h"
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

		static GameObject* GetRow(int x, int y, int w, int h)
		{
			std::vector<GameObject*> children;
			for (int i = 0;i < 5;i++)
			{
				children.push_back(GetButton(0, 0, w, h));
			}

			GameObject* rowObject = new GameObject(x, y, 0, 0);
			Row* row = new Row(rowObject, Config::PADDING, 0, children);

			rowObject->AddComponent(row);
			return rowObject;
		}
	};
}