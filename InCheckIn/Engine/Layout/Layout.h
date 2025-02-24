#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include "../Component.h"

namespace Engine
{
	class Layout : public Component
	{
	public:
		Layout(int padding, int margin, int x, int y, std::vector<Component*> components) :
			Component(x, y, 0, 0), padding(padding), margin(margin)
		{
			SetRelSize(margin * 2, margin * 2);
		}

		void AddComponent(Component& child)
		{
			if (!this->AdoptChild(&child)) return;

			const SDL_Rect* myRect = GetAbsTf();
			const SDL_Rect* objRect = child.GetAbsTf();

			StretchContainer(objRect, myRect);
		}

		void Render(SDL_Surface* surface) override
		{
			for (const auto& component : GetChildren())
			{
				component->Render(surface);
			}
		}

		void HandleEvent(const SDL_Event& event) override
		{
			for (const auto& component : GetChildren())
			{
				component->HandleEvent(event);
			}
		}

		virtual ~Layout() = default;

	protected:
		int GetPadding() { return padding; }
		int GetMargin() { return margin; }

		void InitLayout(std::vector<Component*> components)
		{
			for (Component* component : components)
			{
				AddComponent(*component);
			}

			HandleChildPosition();
		}

		virtual void StretchContainer(const SDL_Rect* objRect,
			const SDL_Rect* myRect) = 0;

	private:
		int padding = 0;
		int margin = 0;
	};
}