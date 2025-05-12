#pragma once
#include <SDL.h>
#include <functional>
#include "Managers/SoundManager.h"
#include "Managers/Globals.h"
#include "Component.h"
#include "GameObject.h"

namespace Engine
{
	//TODO: Make this event a template with null specialization
	class Event
	{
	public:
		Event()
		{
			onInvoke.reserve(Conf::MAX_OBSERVERS);
		}

		void AddEvent(std::function<void()> action)
		{
			onInvoke.push_back(std::move(action));
		}

		void Invoke()
		{
			if (onInvoke.size() < 1) return;

			for (auto& function : onInvoke) function();
		}

		void Clear()
		{
			onInvoke.clear();
		}

	private:
		std::vector<std::function<void()>> onInvoke;
	};

	class Button : public Component
	{
	public:
		Button(GameObject* parent) : Component(parent){}

		void AddOnLeftClick(std::function<void()> leftClick) { onLeftClick.AddEvent(leftClick); }
		void AddOnRightClick(std::function<void()> rightClick) { onRightClick.AddEvent(rightClick); }
		void AddOnHoverEnter(std::function<void()> hoverEnter) { onHoverEnter.AddEvent(hoverEnter); }
		void AddOnHoverExit(std::function<void()> hoverExit) { onHoverExit.AddEvent(hoverExit); }

		void ClearLeftClick() { onLeftClick.Clear(); }
		void ClearRightClick() { onRightClick.Clear(); }
		void SetEnabled(bool state) { isEnabled = state; }

		void HandleEvent(const SDL_Event& event) override
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
						onLeftClick.Invoke() : onRightClick.Invoke();
				}
			}
		}

		void Render(SDL_Surface* surface) override {}

	protected:
		void HandleMouseMotion(const SDL_MouseMotionEvent& event)
		{
			if (parent->IsWithinBounds(event.x, event.y))
			{
				if (!isHovered) onHoverEnter.Invoke();
				isHovered = true;
			}
			else
			{
				if (isHovered) onHoverExit.Invoke();
				isHovered = false;
			}
		}

	private:
		bool isEnabled = true;
		bool isHovered = false;

		Event onLeftClick = Event();
		Event onRightClick = Event();
		Event onHoverEnter = Event();
		Event onHoverExit = Event();
	};
}