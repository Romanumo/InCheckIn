#pragma once
#include <SDL_image.h>
#include <string>

#include "Managers/ResourceManager.h"
#include "Managers/LoadUtils.h"
#include "Managers/Globals.h"

#include "Component.h"

namespace Engine
{
	class Image : public Component
	{
	public:
		Image(int x, int y, int w, int h,
			const std::string& file) :
			Component{ x, y, w, h }
		{
			SetImage(file);
		}

		virtual void HandleEvent(const SDL_Event& event) override {}

		void Render(SDL_Surface* destSurface) override
		{
			SDL_BlitScaled(imageSurface.get(), nullptr, destSurface, GetAbsTf());
		}

		void SetImage(const std::string& filePath)
		{
			imageSurface = ResourceManager<SDL_Surface>::GetInstance().GetByPath(filePath, LoadUtils::LoadImage);
		}

	private:
		std::shared_ptr<SDL_Surface> imageSurface = nullptr;
	};
}