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
		GameObject();
		GameObject(int x, int y, int w, int h);

		SDL_Rect* GetAbsTf();
		const SDL_Rect* GetAbsTf() const;
		const SDL_Rect* GetRelTf() const;

		void SetRelSize(int w, int h);
		void SetRelPosition(int x, int y);
		bool IsWithinBounds(int x, int y) const;

		std::string GetName() const;
		GameObject* GetParent() const; 
		const std::vector<std::unique_ptr<GameObject>>& GetChildren() const;

		void PrintFamilyTree(int spacing = 0);
		void RemoveChild(GameObject* child);
		bool AdoptChild(std::unique_ptr<GameObject> child);
		std::unique_ptr<GameObject> TransferChild(GameObject* child);

		void HandleEvent(const SDL_Event& event) const;
		void Render(SDL_Surface* surface) const;

		virtual ~GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;

		void AddComponent(Component* component);
		template<typename T> T* GetComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			for (const auto& comp : components)
			{
				T* derived = dynamic_cast<T*>(comp.get());
				if (derived) return derived;
			}

			std::cout << "GameObject tried to get non included component" << std::endl;
			return nullptr;
		}
		template<typename T> void RemoveComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			for (int i = 0;i < components.size();i++)
			{
				T* derived = dynamic_cast<T*>(components[i].get());
				if (derived)
				{
					components.erase(components.begin() + i);
					return;
				}
			}

			std::cout << "GameObject tried to remove non included component" << std::endl;
		}

	protected:
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
		void HandleChildPosition();

		bool CheckChildInheritance(const GameObject* child);
	};
}