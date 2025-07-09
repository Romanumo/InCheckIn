#pragma once
#include <SDL.h>
#include "GameObject.h"

namespace Engine
{
	class Component
	{
	public:
		Component() {}
		Component(GameObject* parent)
		{
			this->parent = parent;
		}

		GameObject* GetParent() { return parent; }

		virtual void Render(SDL_Surface* surface) = 0;
		virtual void HandleEvent(const SDL_Event& event) = 0;

		virtual ~Component() = default;

	protected:
		GameObject* parent = nullptr;
	};
}