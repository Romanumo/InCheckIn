#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>

namespace LoadUtils
{
	static std::shared_ptr<SDL_Surface> LoadText(const std::string& text, TTF_Font* font, SDL_Color color)
	{
		SDL_Surface* rawSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
		if (!rawSurface)
		{
			#ifdef SHOW_DEBUG_HELPERS
			Utils::CheckSDLErrors("TEXT_Load");
			#endif // SHOW_DEBUG_HELPERS

			return nullptr;
		}

		return std::shared_ptr<SDL_Surface>(rawSurface, SDL_FreeSurface);
	}

	static std::shared_ptr<TTF_Font> LoadFont(const std::string& path, int fontSize)
	{
		TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
		if (!font)
		{
			#ifdef SHOW_DEBUG_HELPERS
			Utils::CheckSDLErrors("TTF_OpenFont");
			#endif // SHOW_DEBUG_HELPERS

			return nullptr;
		}

		return std::shared_ptr<TTF_Font>(font, TTF_CloseFont);
	}

	static std::shared_ptr<SDL_Surface> LoadImage(const std::string& filePath)
	{
		SDL_Surface* rawSurface = IMG_Load(filePath.c_str());
		if (!rawSurface)
		{
			#ifdef SHOW_DEBUG_HELPERS
			Utils::CheckSDLErrors("IMG_Load");
			#endif // SHOW_DEBUG_HELPERS

			return nullptr;
		}
		return std::shared_ptr<SDL_Surface>(rawSurface, SDL_FreeSurface);
	}
}