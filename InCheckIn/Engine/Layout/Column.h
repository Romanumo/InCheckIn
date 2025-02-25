#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include "Layout.h"

namespace Engine
{
	class Column : public Layout
	{
	public:
		Column(GameObject* parent, int padding, int margin, std::vector<GameObject*> children) :
			Layout(parent, padding, margin, children)
		{
			InitLayout(children);
		}

	protected:
		void HandleChildPosition() override
		{
			if (parent->GetChildren().size() < 1) return;

			int yLength = GetMargin();

			for (const auto& gameObject : parent->GetChildren())
			{
				gameObject->SetRelPosition(GetMargin(), yLength);
				yLength += gameObject->GetAbsTf()->h + GetPadding();
			}
		}

		void StretchContainer(const SDL_Rect* objRect,
			const SDL_Rect* myRect) override
		{
			int updatedW = 0;
			int updatedH = 0;

			updatedH = objRect->h + myRect->h + GetPadding();
			if (objRect->w > myRect->w)
			{
				updatedW = objRect->w;
			}

			parent->SetRelSize(updatedW, updatedH);
		}
	};
}