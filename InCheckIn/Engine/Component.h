#pragma once
#include <SDL.h>
#include <typeinfo>
#include <string>
#include <memory>
#include "Managers/Globals.h"

namespace Engine
{
	class Component
	{
	public:
		Component(int x, int y, int w, int h);

		SDL_Rect* GetAbsTf();
		const SDL_Rect* GetAbsTf() const;

		void SetRelSize(int w, int h);
		void SetRelPosition(int x, int y);

		std::string GetName() const;
		Component* GetParent() const; 
		const std::vector<std::unique_ptr<Component>>& GetChildren() const;

		bool AdoptChild(Component* child);
		void PrintFamilyTree(int spacing = 0);

		virtual void Render(SDL_Surface* surface) = 0;
		virtual void HandleEvent(const SDL_Event& event) = 0;

		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		virtual ~Component() = default;

	protected:
		virtual void HandleChildPosition();

		void ReserveChildrenSize(int reserve);
		bool IsMyChild(const Component& child) const;
		bool IsWithinBounds(int x, int y) const;

	private:
		SDL_Rect absTf{ 0,0,0,0 };
		SDL_Rect relTf = absTf;

		Component* parent = nullptr;
		std::vector<std::unique_ptr<Component>> children;

		void UpdateAbsTf();
		void UpdateTransform();
	};
}