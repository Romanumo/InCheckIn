#pragma once
#define SHOW_DEBUG_HELPERS

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
	inline constexpr int CARD_HEIGHT = 150;
	inline constexpr int CARD_WIDTH = 100;
	inline constexpr int SIDE_MAX_CARDS = 6;

	inline constexpr int TABLE_WIDTH = (CARD_WIDTH + PADDING) * SIDE_MAX_CARDS + PADDING;
	inline constexpr int TABLE_HEIGHT = (CARD_HEIGHT + PADDING) * 3 + PADDING;

	inline constexpr int WINDOW_HEIGHT = TABLE_HEIGHT;
	inline constexpr int WINDOW_WIDTH = TABLE_WIDTH + 300;

	//Colors
	inline constexpr SDL_Color BACKGROUND_COLOR{ 170, 170, 170, 255 };
	inline constexpr SDL_Color BUTTON_COLOR{ 200, 200, 200, 255 };
	inline constexpr SDL_Color BUTTON_HOVER_COLOR{ 220, 220, 220, 255 };
	inline constexpr SDL_Color BUTTON_CLEARED_COLOR{ 240, 240, 240, 255 };
	inline constexpr SDL_Color BUTTON_SUCCESS_COLOR{ 210, 235, 210, 255 };
	inline constexpr SDL_Color BUTTON_FAILURE_COLOR{ 235, 210, 210, 255 };

	//Asset Paths
	inline const std::string PLACEHOLDER_IMAGE = "Assets/Placeholder.png";
	inline const std::string CARD_IMAGE_HEALER = "Assets/Healer.png";
	inline const std::string DESK_IMAGE = "Assets/TableSide.png";
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
			std::cerr << msg << "[ERROR]: " << error << '\n';
			SDL_ClearError();
		}
	}
#endif // SHOW_DEBUG_HELPERS
}