#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include "Layout.h"

namespace Engine
{
	class Row : public Layout
	{
	public:
		Row(GameObject* parent, int padding, int margin, std::vector<GameObject*> children) :
			Layout(parent, padding, margin, children)
		{
			InitLayout(children);
		}

	protected:
		void HandleChildPosition() override
		{
			if (parent->GetChildren().size() < 1) return;

			int xLength = GetMargin();

			for (const auto& gameObject : parent->GetChildren())
			{
				gameObject->SetRelPosition(xLength, GetMargin());
				xLength += gameObject->GetAbsTf()->w + GetPadding();
			}
		}

		void StretchContainer(const SDL_Rect* objRect, 
							const SDL_Rect* myRect) override
		{
			int updatedW = 0;
			int updatedH = 0;

			updatedW = objRect->w + myRect->w + GetPadding();
			if (objRect->h > myRect->h)
			{
				updatedH = objRect->h;
			}

			parent->SetRelSize(updatedW, updatedH);
		}
	};
}