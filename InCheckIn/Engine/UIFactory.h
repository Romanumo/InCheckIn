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

namespace Engine
{
	static class UIFactory
	{
	public:
		static std::unique_ptr<GameObject> NewButton(int x, int y, int w, int h)
		{
			auto button = std::make_unique<GameObject>(x, y, w, h);
			Button* controller = new Button(button.get());
			Rectangle* visual = new Rectangle(button.get());
			visual->SetColor(Conf::BUTTON_COLOR);

			controller->AddOnHoverEnter([visual] {
				visual->SetColor(Conf::BUTTON_HOVER_COLOR);
				});

			controller->AddOnHoverExit([visual] {
				visual->SetColor(Conf::BUTTON_COLOR);
				});

			button->AddComponent(controller);
			button->AddComponent(visual);
			return button;
		}

		static std::unique_ptr<GameObject> NewText(int x, int y, int w, int h,
			const std::string& text)
		{
			auto textObj = std::make_unique<GameObject>(x, y, w, h);
			Text* textComponent = new Engine::Text(textObj.get(), text, { 0,0,0, 255 }, 30);
			textObj->AddComponent(textComponent);
			return textObj;
		}

		static std::unique_ptr<GameObject> NewText(int x, int y, int w, int h,
			Text*& textComponent)
		{
			auto textObj = std::make_unique<GameObject>(x, y, w, h);
			textComponent = new Engine::Text(textObj.get(), " ", {0,0,0, 255}, 30);
			textObj->AddComponent(textComponent);
			return textObj;
		}

		template<typename Type, typename... Args>
		static std::vector<Type*> GetLayout(GameObject* host, LayoutType* type, 
			int count, Args&&... args)
		{
			static_assert(std::is_base_of<GameObject, Type>::value, "Type must derive from GameObject");
			Layout* layout = new Layout(host, type, Conf::PADDING, 0);
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
		static std::unique_ptr<GameObject> NewColumn(Args&&... args)
		{
			return GetLayoutObj(new Column(), std::forward<Args>(args)...);
		}

		template<typename... Args>
		static std::unique_ptr<GameObject> NewRow(Args&&... args)
		{
			return GetLayoutObj(new Row(), std::forward<Args>(args)...);
		}

	private:
		template<typename... Args>
		static std::unique_ptr<GameObject> GetLayoutObj(LayoutType* type, Args&&... args)
		{
			auto compartment = std::make_unique<GameObject>();
			Layout* layout = new Layout(compartment.get(), type, Conf::PADDING, 0);

			(layout->AddGameObject(std::move(args)), ...);

			compartment->AddComponent(layout);
			return compartment;
		}
	};
}