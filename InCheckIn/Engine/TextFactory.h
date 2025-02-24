#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>

#include "Managers/ResourceManager.h"

namespace Engine
{
	class ITextFactory
	{
	public:
		virtual ~ITextFactory() = default;
		virtual std::shared_ptr<SDL_Surface> GetSurface(
			const std::string& text, TTF_Font* font, SDL_Color color) = 0;
	};

	class DynamicTextFactory : public ITextFactory
	{
	public:
		std::shared_ptr<SDL_Surface> GetSurface(
			const std::string& text, TTF_Font* font, SDL_Color color) override
		{
			return LoadUtils::LoadText(text, font, color);
		}
	};

	class CachedTextFactory : public ITextFactory
	{
	public:
		std::shared_ptr<SDL_Surface> GetSurface(
			const std::string& text, TTF_Font* font, SDL_Color color) override
		{
			const std::string colorID = std::to_string(color.r + color.g + color.b);
			const std::string textID = TTF_FontFaceStyleName(font) + text + colorID;
			TTF_Font* usedFont = font;

			auto loadText = [text, usedFont, color]() -> std::shared_ptr<SDL_Surface>
				{return LoadUtils::LoadText(text, usedFont, color);};

			return ResourceManager<SDL_Surface>::GetInstance().
				GetByName(textID, loadText);
		}
	};
}