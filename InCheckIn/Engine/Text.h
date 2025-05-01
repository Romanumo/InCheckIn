#pragma once
#include <SDL_ttf.h>
#include <string>

#include "Managers/Globals.h"
#include "Managers/LoadUtils.h"
#include "TextSurface.h"
#include "Component.h"

namespace Engine
{
	class Text : public Component
	{
	public:
		Text(GameObject* parent, const std::string& text, 
			SDL_Color color = { 0, 0,0,255 }, int fontSize = 30) : 
			Component(parent), textColor(color)
		{
			textFactory = std::make_unique<DynamicTextSurface>();

			const std::string& fontID = Conf::FONT + std::to_string(fontSize);
			auto loadFont = [fontSize]() -> std::shared_ptr<TTF_Font>
				{return LoadUtils::LoadFont(Conf::FONT, fontSize);};

			font = ResourceManager<TTF_Font>::GetInstance().
				GetByName(fontID, loadFont);

			SetText(text, color);
		}

		void SetText(const std::string& text) { SetText(text, textColor); }

		void SetText(const std::string& text, SDL_Color color)
		{
			textColor = color;

			textSurface = textFactory->GetSurface(text, font.get(), color, 
				parent->GetAbsTf()->w * 5/6);

			UpdateTextPosition();
		}

		void SetTextRenderType(std::unique_ptr<ITextSurface> textFactory)
		{
			this->textFactory = std::move(textFactory);
		}

		void Render(SDL_Surface* surface) override
		{
			UpdateTextPosition();
			SDL_BlitScaled(textSurface.get(), nullptr, surface, &textPos);
		}

		virtual void HandleEvent(const SDL_Event& event) override {}

	private:
		std::shared_ptr<SDL_Surface> textSurface = nullptr;
		std::shared_ptr<TTF_Font> font = nullptr;

		SDL_Rect textPos{ 0,0,0,0 };
		SDL_Color textColor{ 0,0,0,255 };

		//Why unique?
		std::unique_ptr<ITextSurface> textFactory;

		void UpdateTextPosition()
		{
			auto [x, y, w, h] = *parent->GetAbsTf();

			//Horizontal Centering
			const int widthDifference = w - textSurface->w;
			const int leftOffset = widthDifference / 2;

			//Vertical Centering
			const int heightDifference = h - textSurface->h;
			const int topOffset = heightDifference / 2;

			textPos = { x + leftOffset, y + topOffset, w - widthDifference, h - heightDifference };
		}
	};
}
