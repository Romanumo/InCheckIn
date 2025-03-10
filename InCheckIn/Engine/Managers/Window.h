#pragma once
#include <SDL.h>
#include "Globals.h"

namespace Engine
{
	class Window
	{
	public:
		Window()
		{
			SDLWindow = SDL_CreateWindow(
				Conf::GAME_NAME.c_str(), 
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, 
				Conf::WINDOW_WIDTH, 
				Conf::WINDOW_HEIGHT, 0);
			//SDL_SetWindowFullscreen(SDLWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}

		void Render()
		{
			SDL_FillRect(GetSurface(), nullptr,
				SDL_MapRGB(
					GetSurface()->format, 
					Conf::BACKGROUND_COLOR.r,
					Conf::BACKGROUND_COLOR.g,
					Conf::BACKGROUND_COLOR.b));
		}

		SDL_Surface* GetSurface()
		{
			return SDL_GetWindowSurface(SDLWindow);
		}

		void UpdateFrame()
		{
			SDL_UpdateWindowSurface(SDLWindow);
		}

		~Window()
		{
			SDL_DestroyWindow(SDLWindow);
		}

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	private:
		SDL_Window* SDLWindow;
	};
}