#pragma once
#include <SDL.h>
#include <typeinfo>
#include <functional>
#include <string>
#include <memory>
#include "Managers/Globals.h"

namespace Engine
{
	class Component;

	class GameObject
	{
	public:
		GameObject(int x, int y, int w, int h);

		SDL_Rect* GetAbsTf();
		const SDL_Rect* GetAbsTf() const;

		void SetRelSize(int w, int h);
		void SetRelPosition(int x, int y);
		bool IsWithinBounds(int x, int y) const;

		std::string GetName() const;
		GameObject* GetParent() const; 
		const std::vector<std::unique_ptr<GameObject>>& GetChildren() const;

		bool AdoptChild(GameObject* child);
		void PrintFamilyTree(int spacing = 0);

		void AddComponent(Component* component);

		void HandleEvent(const SDL_Event& event);
		void Render(SDL_Surface* surface);

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		virtual ~GameObject() = default;

	protected:
		virtual void HandleChildPosition();

		void ReserveChildrenSize(int reserve);
		bool IsMyChild(const GameObject& child) const;

	private:
		SDL_Rect absTf{ 0,0,0,0 };
		SDL_Rect relTf = absTf;

		GameObject* parent = nullptr;
		std::vector<std::unique_ptr<GameObject>> children;
		std::vector<std::unique_ptr<Component>> components;

		void UpdateAbsTf();
		void UpdateTransform();
	};
}