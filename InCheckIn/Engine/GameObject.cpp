#pragma once
#include <SDL.h>
#include <typeinfo>
#include <string>
#include "GameObject.h"
#include "Component.h"
using namespace Engine;

GameObject::GameObject(int x, int y, int w, int h) :
	relTf{ x, y, w, h }, parent(nullptr)
{
	UpdateTransform();
}

#pragma region Positioning

void GameObject::SetRelPosition(int x, int y)
{
	relTf.x = x;
	relTf.y = y;

	UpdateTransform();
}

void GameObject::SetRelSize(int w, int h)
{
	relTf.w = w;
	relTf.h = h;

	UpdateTransform();
}

void GameObject::UpdateTransform()
{
	UpdateAbsTf();
	HandleChildPosition();
}

void GameObject::UpdateAbsTf()
{
	absTf = relTf;

	if (parent != nullptr)
	{
		absTf.x += parent->GetAbsTf()->x;
		absTf.y += parent->GetAbsTf()->y;
	}
}

void GameObject::HandleChildPosition()
{
	if (children.size() < 1) return;

	for (const auto& gameObject : children)
	{
		gameObject->UpdateTransform();
	}
}

#pragma endregion

#pragma region FamilyFunctions

void GameObject::PrintFamilyTree(int spacing)
{
	std::string offset(spacing, '-');
	std::cout << offset << GetName() << std::endl;

	if (children.size() < 1) return;

	for (const auto& component : children)
	{
		component->PrintFamilyTree(spacing + 1);
	}
}

bool GameObject::AdoptChild(GameObject* child)
{
	if (child == nullptr)
	{
		std::cout << "Cannot adopt a nullptr as child!" << std::endl;
		return false;
	}

	if (child == this)
	{
		std::cout << "Parent cannot adopt himself" << std::endl;
		return false;
	}

	if (child->GetParent() != nullptr)
	{
		std::cout << "GameObject already have a parent" << std::endl;
		return false;
	}

	if (child->IsMyChild(*this))
	{
		std::cout << "GameObject ancestor is his child" << std::endl;
		return false;
	}

	child->parent = this;
	children.push_back(std::unique_ptr<GameObject>(child));
	return true;
}

bool GameObject::IsMyChild(const GameObject& child) const
{
	if (children.size() < 1) return false;

	for (const auto& component : children)
	{
		if (component.get() == &child) return true;

		if (component->IsMyChild(child)) return true;
	}

	return false;
}

void GameObject::ReserveChildrenSize(int reserve) { children.reserve(reserve); }

#pragma endregion

#pragma region GettersSetters

std::string GameObject::GetName() const { return typeid(*this).name(); }
GameObject* GameObject::GetParent() const { return parent; }
const std::vector<std::unique_ptr<GameObject>>& GameObject::GetChildren() const { return children; }

const SDL_Rect* GameObject::GetAbsTf() const { return &absTf; }
SDL_Rect* GameObject::GetAbsTf() { return &absTf; }

#pragma endregion

void GameObject::AddComponent(Component* component)
{
	for (const auto& comp : components)
	{
		if (typeid(*component) == typeid(*comp))
		{
			std::cout << "GameObject cant have duplicates of the same component type" << std::endl;
			return;
		}
	}

	components.push_back(std::unique_ptr<Component>(component));
}

template<typename T>
T* GameObject::GetComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

	for (const auto& comp : components)
	{
		T* derived = dynamic_cast<T*>(comp);
		if (derived) return derived;
	}

	std::cout << "GameObject tried to get non included component" << std::endl;
	return nullptr;
}

void GameObject::HandleEvent(const SDL_Event& event)
{
	for (auto& component : components)
	{
		component->HandleEvent(event);
	}

	if (children.size() < 1) return;

	for (const auto& gameObject : children)
	{
		gameObject->HandleEvent(event);
	}
}

void GameObject::Render(SDL_Surface* surface)
{
	for (auto& component : components)
	{
		component->Render(surface);
	}

	if (children.size() < 1) return;

	for (const auto& gameObject : children)
	{
		gameObject->Render(surface);
	}
}

bool GameObject::IsWithinBounds(int x, int y) const
{
	if (x < absTf.x) return false;
	if (y < absTf.y) return false;
	if (x > absTf.x + absTf.w) return false;
	if (y > absTf.y + absTf.h) return false;

	return true;
}

//POssess a Link bug
/*template<typename T>
void GameObject::RemoveComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

	for (int i = 0;i < components.size();i++)
	{
		T* derived = dynamic_cast<T*>(components[i].get());
		if (derived)
		{
			std::cout << typeid(*components.at(i)).name() << std::endl;
			components.erase(components.begin() + i);
			return;
		}
	}

	std::cout << "GameObject tried to remove non included component" << std::endl;
}*/