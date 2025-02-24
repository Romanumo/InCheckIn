#pragma once
#include <SDL.h>

#include "Managers/Globals.h"
#include "Component.h"

namespace Engine
{
	class Rectangle : public Component
	{
	public:
		Rectangle(int x, int y, int w, int h,
			SDL_Color color = { 0,0,0, 255 }) :
			Component{ x, y, w, h }, color(color) {
		}

		virtual void Render(SDL_Surface* surface) override
		{
			SDL_FillRect(surface, GetAbsTf(), SDL_MapRGB(
				surface->format,
				color.r, color.g, color.b));
		}

		virtual void HandleEvent(const SDL_Event& event) override {}

		void SetColor(SDL_Color col) { color = col; }

	private:
		SDL_Color color{ 0,0,0,0 };
	};
}