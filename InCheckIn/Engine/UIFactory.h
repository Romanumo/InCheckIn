#pragma once
#include <SDL.h>
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

			controller->AddOnHover([visual](bool isHovered) {
				if (isHovered)
				{
					visual->SetColor(Config::BUTTON_HOVER_COLOR);
				}
				else
				{
					visual->SetColor(Config::BUTTON_COLOR);
				}
				});

			button->AddComponent(controller);
			button->AddComponent(visual);
			return button;
		}
	};
}