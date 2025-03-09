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

		template<typename Type, typename... Args>
		static std::vector<Type*> GetLayout(GameObject* host, LayoutType* type, 
			int count, Args&&... args)
		{
			static_assert(std::is_base_of<GameObject, Type>::value, "Type must derive from GameObject");
			Layout* layout = new Layout(host, type, Config::PADDING, 0);
			std::vector<Type*> pointers;

			for (int i = 0;i < count;i++)
			{
				auto obj = std::make_unique<Type>(std::forward<Args>(args)...);
				pointers.push_back(obj.get());
				layout->AddGameObject(std::move(obj));
			}

			host->AddComponent(layout);
			return pointers;
		}

		template<typename... Args>
		static std::unique_ptr<GameObject> GetColumn(Args&&... args)
		{
			return GetLayoutObj(new Column(), std::forward<Args>(args)...);
		}

		template<typename... Args>
		static std::unique_ptr<GameObject> GetRow(Args&&... args)
		{
			return GetLayoutObj(new Row(), std::forward<Args>(args)...);
		}

	private:
		template<typename... Args>
		static std::unique_ptr<GameObject> GetLayoutObj(LayoutType* type, Args&&... args)
		{
			auto compartment = std::make_unique<GameObject>();
			Layout* layout = new Layout(compartment.get(), type, Config::PADDING, 0);

			(layout->AddGameObject(std::move(args)), ...);

			compartment->AddComponent(layout);
			return compartment;
		}
	};
}