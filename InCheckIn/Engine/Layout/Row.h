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
			const auto& children = parent->GetChildren();
			if (children.empty()) return;

			int totalWidth = -GetPadding();
			for (const auto& gameObject : children)
			{
				totalWidth += gameObject->GetAbsTf()->w + GetPadding();
			}

			int offset = (parent->GetAbsTf()->w - totalWidth)/2;
			for (const auto& gameObject : children)
			{
				gameObject->SetRelPosition(offset, GetMargin());
				offset += gameObject->GetAbsTf()->w + GetPadding();
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

			updatedW += GetPadding() + objRect->w;
			if (objRect->h > myRect->h)
			{
				updatedH = objRect->h;
			}

			parent->SetRelSize(updatedW, updatedH);
		}
	};
}