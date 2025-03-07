#pragma once
#include <SDL.h>
#include <memory>

#include "Layout/Layout.h"

#include "Button.h"
#include "Image.h"
#include "Text.h"
#include "TextSurface.h"
#include "Rectangle.h"
#include "GameObject.h"
#include "Component.h"

#include "../Card.h"

namespace Engine
{
	static class UIFactory
	{
	public:
		static std::unique_ptr<GameObject> GetButton(int x, int y, int w, int h)
		{
			auto button = std::make_unique<GameObject>(x, y, w, h);
			Button* controller = new Button(button.get());
			Rectangle* visual = new Rectangle(button.get());
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

		/*template<typename Type, typename... Args>
		static std::vector<Type*> GetRow(GameObject* host, int count, Args&&... args)
		{
			static_assert(std::is_base_of<GameObject, Type>::value, "Type must derive from GameObject");
			std::vector<Type*> pointers;

			for (int i = 0;i < count;i++)
			{
				Type* obj = new Type(std::forward<Args>(args)...);
				pointers.push_back(obj);
			}

			Row* rowComponent = new Row(host, Config::PADDING, 0, args);
			host->AddComponent(rowComponent);

			return pointers;
		}*/

		/*template<typename Type, typename... Args>
		static std::vector<Type*> GetColumn(GameObject* host, int count, Args&&... args)
		{
			std::vector<std::unique_ptr<Type>> children;

			for (int i = 0;i < count;i++)
			{
				children.push_back(std::make_unique<Type>(std::forward<Args>(args)...));
			}

			Column* columnComponent = new Column(host, Config::PADDING, 0, std::move(children));
			host->AddComponent(columnComponent);
		}*/
	};
}