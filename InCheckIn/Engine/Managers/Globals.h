#pragma once
//#define SHOW_DEBUG_HELPERS

#include<iostream>
#include<SDL.h>
#include<vector>
#include<string>

namespace Config
{
	//Game Settings
	inline const std::string GAME_NAME = "InCheckIn";

	//Size and Positioning
	inline constexpr int PADDING = 5;

	inline constexpr int WINDOW_HEIGHT = 600;
	inline constexpr int WINDOW_WIDTH = 1200;

	//Colors
	inline constexpr SDL_Color BACKGROUND_COLOR{ 170, 170, 170, 255 };
	inline constexpr SDL_Color BUTTON_COLOR{ 200, 200, 200, 255 };
	inline constexpr SDL_Color BUTTON_HOVER_COLOR{ 220, 220, 220, 255 };
	inline constexpr SDL_Color BUTTON_CLEARED_COLOR{ 240, 240, 240, 255 };
	inline constexpr SDL_Color BUTTON_SUCCESS_COLOR{ 210, 235, 210, 255 };
	inline constexpr SDL_Color BUTTON_FAILURE_COLOR{ 235, 210, 210, 255 };

	//Asset Paths
	inline const std::string FONT = "Assets/Rubik-SemiBold.ttf";

	//Max Chilren
	inline const int MAX_CHILDREN = 20;
	inline const int MAX_OBSERVERS = 5;
}

namespace Utils
{
#ifdef SHOW_DEBUG_HELPERS
	inline void CheckSDLErrors(const std::string& msg)
	{
		const char* error = SDL_GetError();
		if (*error != '\0')
		{
			std::cerr << msg << "Error: " << error << '\n';
			SDL_ClearError();
		}
	}
#endif // SHOW_DEBUG_HELPERS
}