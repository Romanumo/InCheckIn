#pragma once
#include <SDL.h>
#include <functional>
#include "Managers/SoundManager.h"
#include "Managers/Globals.h"
#include "Component.h"
#include "GameObject.h"

namespace Engine
{
	class Button : public Component
	{
	public:
		Button(GameObject* parent) :
			Component(parent){
		}

		void AddOnLeftClick(std::function<void()> leftClick) { onLeftClick.push_back(std::move(leftClick)); }
		void AddOnRightClick(std::function<void()> rightClick) { onRightClick.push_back(std::move(rightClick)); }
		void AddOnHover(std::function<void(bool)> hover) { onHover.push_back(std::move(hover)); }
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
				if (parent->IsWithinBounds(event.button.x, event.button.y))
				{
					event.button.button == SDL_BUTTON_LEFT ? 
						HandleLeftClick() : HandleRightClick();
				}
			}
		}

		void Render(SDL_Surface* surface) override {}

	protected:
		void HandleMouseMotion(const SDL_MouseMotionEvent& event)
		{
			isHovered = parent->IsWithinBounds(event.x, event.y);

			HandleHover();
		}

	private:
		bool isEnabled = true;
		bool isHovered = true;

		std::vector<std::function<void()>> onLeftClick;
		std::vector<std::function<void()>> onRightClick;
		std::vector<std::function<void(bool)>> onHover;

		void HandleLeftClick()
		{
			if (onLeftClick.size() < 1) return;

			for (auto& function : onLeftClick) function();
			//SoundManager::GetInstance().PlaySFX();
		}

		void HandleRightClick()
		{
			if (onRightClick.size() < 1) return;

			for (auto& function : onRightClick) function();
			//SoundManager::GetInstance().PlaySFX();
		}

		void HandleHover()
		{
			if (onHover.size() < 1) return;

			for (auto& function : onHover) function(isHovered);
		}
	};
}