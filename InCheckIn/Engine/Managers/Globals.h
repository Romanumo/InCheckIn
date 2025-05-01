#pragma once
#define SHOW_DEBUG_HELPERS

#include<iostream>
#include<SDL.h>
#include<vector>
#include<string>

namespace Conf
{
	//Game Settings
	inline const std::string GAME_NAME = "The Loop";
	inline constexpr int MAX_HAND_SIZE = 4;

	//Size and Positioning
	inline constexpr int PADDING = 5;
	inline constexpr int CARD_HEIGHT = 150;
	inline constexpr int CARD_WIDTH = 100;
	inline constexpr int MAX_CARDS = 6;

	inline constexpr int AVATAR_WIDTH = CARD_WIDTH * 3;
	inline constexpr int AVATAR_HEIGHT = CARD_HEIGHT;

	inline constexpr int TURNBT_WIDTH = AVATAR_WIDTH;
	inline constexpr int TURNBT_HEIGHT = AVATAR_HEIGHT;

	inline constexpr int TABLE_WIDTH = (CARD_WIDTH + PADDING) * MAX_CARDS - PADDING;
	inline constexpr int TABLE_HEIGHT = (CARD_HEIGHT + PADDING) * 3 + PADDING;

	inline constexpr int WINDOW_HEIGHT = TABLE_HEIGHT;
	inline constexpr int WINDOW_WIDTH = TABLE_WIDTH + 400;

	//Colors
	inline constexpr SDL_Color BACKGROUND_COLOR{ 170, 170, 170, 255 };
	inline constexpr SDL_Color BUTTON_COLOR{ 200, 200, 200, 255 };
	inline constexpr SDL_Color BUTTON_HOVER_COLOR{ 220, 220, 220, 255 };
	inline constexpr SDL_Color BUTTON_CLEARED_COLOR{ 240, 240, 240, 255 };
	inline constexpr SDL_Color BUTTON_SUCCESS_COLOR{ 210, 235, 210, 255 };
	inline constexpr SDL_Color BUTTON_FAILURE_COLOR{ 235, 210, 210, 255 };
	inline constexpr SDL_Color SPIRAL_COLOR{ 100, 100, 255, 255 };

	//Asset Paths
	inline const std::string FONT = "Assets/Font.ttf";

	inline const std::string SLOT_IMAGE = "Assets/Slot.png";
	inline const std::string SLOT_SELECTED_IMAGE = "Assets/SlotSelected.png";
	inline const std::string TURNBT_IMAGE = "Assets/Turn.png";
	inline const std::string FRAME_IMAGE = "Assets/Frame.png";
	inline const std::string SPIRAL_IMAGE = "Assets/Spiral.png";

	inline const std::string HAMMER_ON_IMAGE = "Assets/HammerUse.png";
	inline const std::string HAMMER_OFF_IMAGE = "Assets/Hammer.png";

	//Units Assets
	inline const std::string CARD_IMAGE_BASIC = "Assets/Basic.png";
	inline const std::string CARD_IMAGE_LEFTY = "Assets/Lefty.png";
	inline const std::string CARD_IMAGE_RIGHTY = "Assets/Righty.png";
	inline const std::string CARD_IMAGE_SUN = "Assets/Sun.png";
	inline const std::string CARD_IMAGE_OBSESSION = "Assets/Obsession.png";
	inline const std::string CARD_IMAGE_INDICATOR = "Assets/Indicator.png";
	inline const std::string CARD_IMAGE_PROTOTYPE = "Assets/Prototype.png";

	//Max Chilren
	inline constexpr int MAX_CHILDREN = 20;
	inline constexpr int MAX_OBSERVERS = 5;

	//Animation Setting
	inline constexpr int NEW_CARD_T = 300;
	inline constexpr int CARD_ANIM_T = 300;
	inline constexpr int PAUSE_TIME = 100;
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