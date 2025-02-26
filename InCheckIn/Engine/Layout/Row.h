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

		void AlignOnCenter() override
		{
			if (parent->GetChildren().size() < 1) return;

			int xLength = 0;
			for (const auto& gameObject : parent->GetChildren())
			{
				xLength += gameObject->GetAbsTf()->w + GetPadding();
			}

			int offset = parent->GetAbsTf()->w - xLength - GetPadding();
			offset /= 2;
			xLength = 0;
			for (const auto& gameObject : parent->GetChildren())
			{
				gameObject->SetRelPosition(offset + xLength, GetMargin());
				xLength += gameObject->GetAbsTf()->w + GetPadding();
			}
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
			int updatedW = myRect->w;
			int updatedH = myRect->h;

			updatedW = objRect->w + myRect->w + GetPadding();
			if (objRect->h > myRect->h)
			{
				updatedH = objRect->h;
			}

			parent->SetRelSize(updatedW, updatedH);
		}
	};
}