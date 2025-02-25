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
		Layout(GameObject *parent, int padding, int margin, std::vector<GameObject*> children) :
			Component(parent), padding(padding), margin(margin)
		{
			parent->SetRelSize(margin * 2, margin * 2);
		}

		void AddComponent(GameObject& child)
		{
			if (!parent->AdoptChild(&child)) return;

			const SDL_Rect* myRect = parent->GetAbsTf();
			const SDL_Rect* objRect = child.GetAbsTf();

			StretchContainer(objRect, myRect);
		}

		/*void Render(SDL_Surface* surface) override
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
		}*/

		virtual ~Layout() = default;

	protected:
		int GetPadding() { return padding; }
		int GetMargin() { return margin; }

		void InitLayout(std::vector<GameObject*> children)
		{
			for (auto& component : children)
			{
				AddComponent(*component);
			}

			HandleChildPosition();
		}

		virtual void HandleChildPosition() = 0;

		virtual void StretchContainer(const SDL_Rect* objRect,
			const SDL_Rect* myRect) = 0;

	private:
		int padding = 0;
		int margin = 0;
	};
}