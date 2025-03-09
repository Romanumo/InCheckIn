#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <SDL.h>
#include "../Component.h"

namespace Engine
{
	class LayoutType
	{
	public:
		virtual void HandleChildPosition(GameObject* parent,  
			int padding, int margin, int offset = 0) = 0;
		virtual int GetDimension(const SDL_Rect& rect) = 0;
	};

	class Row : public LayoutType
	{
		void HandleChildPosition(GameObject* parent,
			int padding, int margin, int offset = 0) override
		{
			int xLength = margin + offset;
			int yLength = 0;

			for (const auto& gameObject : parent->GetChildren())
			{
				gameObject->SetRelPosition(xLength, margin);
				xLength += gameObject->GetAbsTf()->w + padding;

				if (gameObject->GetAbsTf()->h > yLength)
				{
					yLength = gameObject->GetAbsTf()->h;
				}
			}

			if(offset == 0) parent->SetRelSize(xLength - padding, yLength);
		}

		int GetDimension(const SDL_Rect& rect) override
		{
			return rect.w;
		}
	};

	class Column : public LayoutType
	{
		void HandleChildPosition(GameObject* parent,
			int padding, int margin, int offset = 0) override
		{
			int xLength = 0;
			int yLength = margin + offset;

			for (const auto& gameObject : parent->GetChildren())
			{
				gameObject->SetRelPosition(margin, yLength);
				yLength += gameObject->GetAbsTf()->h + padding;

				if (gameObject->GetAbsTf()->w > xLength)
				{
					xLength = gameObject->GetAbsTf()->w;
				}
			}

			if (offset == 0) parent->SetRelSize(xLength, yLength);
		}

		int GetDimension(const SDL_Rect& rect) override
		{
			return rect.h;
		}
	};
}