#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include "LayoutType.h"
#include "../Component.h"

namespace Engine
{
	class Layout : public Component
	{
	public:
		Layout(GameObject *parent, LayoutType* type, int padding, int margin) :
			Component(parent), type(type), padding(padding), margin(margin)
		{
			parent->SetRelSize(margin * 2, margin * 2);
		}

		void AddGameObject(std::unique_ptr<GameObject> child)
		{
			const SDL_Rect* objRect = child->GetAbsTf();
			const SDL_Rect* myRect = parent->GetAbsTf();

			if (!parent->AdoptChild(std::move(child))) return;

			type->HandleChildPosition(parent, padding, margin);
		}

		void AlignCenter()
		{
			if (parent->GetChildren().empty()) return;

			int offset = -padding;
			for (const auto& gameObject : parent->GetChildren())
			{
				offset += type->GetDimension(*gameObject->GetAbsTf()) + padding;
			}
			offset = (type->GetDimension(*parent->GetAbsTf()) - offset) / 2;

			type->HandleChildPosition(parent, padding, margin, offset);
		}

		void Render(SDL_Surface* surface) override {}
		void HandleEvent(const SDL_Event& event) override {}

	private:
		LayoutType* type;
		int padding = 0;
		int margin = 0;
	};
}