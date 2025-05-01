#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>

#include "Managers/ResourceManager.h"

namespace Engine
{
	class ITextSurface
	{
	public:
		virtual ~ITextSurface() = default;
		virtual std::shared_ptr<SDL_Surface> GetSurface(
			const std::string& text, TTF_Font* font, SDL_Color color, int width) = 0;
	};

	class DynamicTextSurface : public ITextSurface
	{
	public:
		std::shared_ptr<SDL_Surface> GetSurface(
			const std::string& text, TTF_Font* font, SDL_Color color, int width) override
		{
			return LoadUtils::LoadText(text, font, color, width);
		}
	};

	class CachedTextSurface : public ITextSurface
	{
	public:
		std::shared_ptr<SDL_Surface> GetSurface(
			const std::string& text, TTF_Font* font, SDL_Color color, int width) override
		{
			const std::string colorID = std::to_string(color.r + color.g + color.b);
			const std::string textID = TTF_FontFaceStyleName(font) + text + colorID;
			TTF_Font* usedFont = font;

			auto loadText = [=]() -> std::shared_ptr<SDL_Surface>
				{return LoadUtils::LoadText(text, usedFont, color, width);};

			return ResourceManager<SDL_Surface>::GetInstance().
				GetByName(textID, loadText);
		}
	};
}