#pragma once
//#define SHOW_DEBUG_HELPERS

#include<iostream>
#include<SDL.h>
#include<vector>
#include<string>

namespace UserEvents 
{
	inline Uint32 CELL_CLEARED = SDL_RegisterEvents(1);

	inline Uint32 GAME_WON = SDL_RegisterEvents(1);
	inline Uint32 GAME_LOST = SDL_RegisterEvents(1);
	inline Uint32 NEW_GAME = SDL_RegisterEvents(1);
}

namespace Config
{
	//Game Settings
	inline const std::string GAME_NAME = "MineSweeper";
	inline constexpr int GRID_COLUMNS = 8;
	inline constexpr int GRID_ROWS = 6;
	inline constexpr int BOMB_COUNT = 10;
	static_assert(
		BOMB_COUNT < GRID_COLUMNS * GRID_ROWS - 9, 
		"Cannot have more bombs than cells");

	//Size and Positioning
	inline constexpr int PADDING = 5;
	inline constexpr int CELL_SIZE = 50;

	inline constexpr int FOOTER_HEIGHT = 60;
	inline constexpr int COUNTER_WIDTH = 100;

	inline constexpr int GRID_HEIGHT =
		GRID_ROWS * CELL_SIZE + PADDING * (GRID_ROWS - 1);

	inline constexpr int GRID_WIDTH =
		GRID_COLUMNS * CELL_SIZE + PADDING * (GRID_COLUMNS - 1);

	inline constexpr int WINDOW_HEIGHT = GRID_HEIGHT + PADDING * 2 + FOOTER_HEIGHT;
	inline constexpr int WINDOW_WIDTH = GRID_WIDTH + PADDING * 2;

	//Colors
	inline constexpr SDL_Color BACKGROUND_COLOR{ 170, 170, 170, 255 };
	inline constexpr SDL_Color BUTTON_COLOR{ 200, 200, 200, 255 };
	inline constexpr SDL_Color BUTTON_HOVER_COLOR{ 220, 220, 220, 255 };
	inline constexpr SDL_Color BUTTON_CLEARED_COLOR{ 240, 240, 240, 255 };
	inline constexpr SDL_Color BUTTON_SUCCESS_COLOR{ 210, 235, 210, 255 };
	inline constexpr SDL_Color BUTTON_FAILURE_COLOR{ 235, 210, 210, 255 };
	inline constexpr SDL_Color COUNTER_COLOR{ 80, 80, 80, 255 };

	inline const std::vector<SDL_Color> TEXT_COLORS{
		/* 0 */ {0, 0, 0, 255}, // Unused
		/* 1 */ {0, 1, 249, 255},
		/* 2 */ {1, 126, 1, 255},
		/* 3 */ {250, 1, 2, 255},
		/* 4 */ {1, 0, 128, 255},
		/* 5 */ {129, 1, 0, 255},
		/* 6 */ {0, 128, 128, 255},
		/* 7 */ {0, 0, 0, 255},
		/* 8 */ {128, 128, 128, 255}
	};

	//Asset Paths
	inline const std::string FONT = "Assets/Rubik-SemiBold.ttf";
	inline const std::string BOMB_IMAGE = "Assets/Bomb.png";
	inline const std::string FLAG_IMAGE = "Assets/Flag.png";
	inline const std::string CLICK_SOUND = "Assets/Click.wav";
	inline const std::string BCG_MUSIC = "Assets/BcgMusic.mp3";
	inline const std::string BOMB_SOUND = "Assets/BombSound.wav";
	inline const std::string FLAG_SOUND = "Assets/FlagSound.mp3";

	//Layout
	inline const int MAX_CHILDREN = 20;
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