#pragma once
#include <SDL.h>
#include <functional>
#include "Managers/SoundManager.h"
#include "Managers/Globals.h"
#include "Rectangle.h"

namespace Engine
{
	class Button : public Rectangle
	{
	public:
		using Callback = std::function<void()>;

		Button(int x, int y, int w, int h) :
			Rectangle(x, y, w, h, Config::BUTTON_COLOR){
		}

		void SetOnLeftClick(Callback leftClick) { onLeftClick = std::move(leftClick); }
		void SetOnRightClick(Callback rightClick) { onRightClick = std::move(rightClick); }
		void SetOnHover(Callback hover) { onHover = std::move(hover); }
		void SetEnabled(bool state) { isEnabled = state; }

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

	protected:
		void HandleLeftClick() 
		{
			if (onLeftClick) onLeftClick();
			//SoundManager::GetInstance().PlaySFX();
		}

		void HandleRightClick() 
		{
			if (onRightClick) onRightClick();
			//SoundManager::GetInstance().PlaySFX();
		}

		void HandleMouseMotion(const SDL_MouseMotionEvent& event)
		{
			if (IsWithinBounds(event.x, event.y))
			{
				SetColor(Config::BUTTON_HOVER_COLOR);
				if(onHover && !isHovered) onHover();
				isHovered = true;
			}
			else
			{
				SetColor(Config::BUTTON_COLOR);
				isHovered = false;
			}
		}

	private:
		bool isEnabled = true;
		bool isHovered = true;

		Callback onLeftClick = nullptr;
		Callback onRightClick = nullptr;
		Callback onHover = nullptr;
	};
}