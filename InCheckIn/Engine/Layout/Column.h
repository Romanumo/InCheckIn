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

		void AlignOnCenter() override
		{
			const auto& children = parent->GetChildren();
			if (children.empty()) return;

			int totalHeight = -GetPadding();
			for (const auto& gameObject : children)
			{
				totalHeight += gameObject->GetAbsTf()->h + GetPadding();
			}

			int offset = (parent->GetAbsTf()->h - totalHeight) / 2;
			for (const auto& gameObject : children)
			{
				gameObject->SetRelPosition(GetMargin(), offset);
				offset += gameObject->GetAbsTf()->h + GetPadding();
			}
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
			int updatedW = myRect->w;
			int updatedH = myRect->h;

			updatedH += objRect->h + GetPadding();
			if (objRect->w > myRect->w)
			{
				updatedW = objRect->w;
			}

			parent->SetRelSize(updatedW, updatedH);
		}
	};
}