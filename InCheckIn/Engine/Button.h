#pragma once
#include <SDL.h>
#include "Managers/SoundManager.h"
#include "Managers/Globals.h"
#include "Rectangle.h"

namespace Engine
{
	class Button : public Rectangle
	{
	public:
		Button(int x, int y, int w, int h) :
			Rectangle(x, y, w, h, Config::BUTTON_COLOR){
		}

		virtual void HandleEvent(const SDL_Event& event) override
		{
			if (!isEnabled) return;

			if (event.type == SDL_MOUSEMOTION)
			{
				HandleMouseMotion(event.motion);
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (IsWithinBounds(event.button.x, event.button.y))
				{
					event.button.button == SDL_BUTTON_LEFT ? 
						HandleLeftClick() : HandleRightClick();
				}
			}
		}

		void SetEnabled(bool state)
		{
			isEnabled = state;
		}

	protected:

		virtual void HandleLeftClick() {}
		virtual void HandleRightClick() {}

		void HandleMouseMotion(const SDL_MouseMotionEvent& event)
		{
			if (IsWithinBounds(event.x, event.y))
			{
				SetColor(Config::BUTTON_HOVER_COLOR);
			}
			else
			{
				SetColor(Config::BUTTON_COLOR);
			}
		}

	private:
		bool isEnabled = true;
	};
}