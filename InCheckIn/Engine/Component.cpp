#pragma once
#include <SDL.h>
#include <typeinfo>
#include <string>
#include "Component.h"
using namespace Engine;

Component::Component(int x, int y, int w, int h) :
	relTf{ x, y, w, h }, parent(nullptr)
{
	UpdateTransform();
}

#pragma region Positioning

void Component::SetRelPosition(int x, int y)
{
	relTf.x = x;
	relTf.y = y;

	UpdateTransform();
}

void Component::SetRelSize(int w, int h)
{
	relTf.w = w;
	relTf.h = h;

	UpdateTransform();
}

void Component::UpdateTransform()
{
	UpdateAbsTf();
	HandleChildPosition();
}

void Component::UpdateAbsTf()
{
	absTf = relTf;

	if (parent != nullptr)
	{
		absTf.x += parent->GetAbsTf()->x;
		absTf.y += parent->GetAbsTf()->y;
	}
}

void Component::HandleChildPosition()
{
	if (children.size() < 1) return;

	for (const auto& component : children)
	{
		component->UpdateTransform();
	}
}

#pragma endregion

#pragma region FamilyFunctions

void Component::PrintFamilyTree(int spacing)
{
	std::string offset(spacing, '-');
	std::cout << offset << GetName() << std::endl;

	if (children.size() < 1) return;

	for (const auto& component : children)
	{
		component->PrintFamilyTree(spacing + 1);
	}
}

bool Component::AdoptChild(Component* child)
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
		std::cout << "Component already have a parent" << std::endl;
		return false;
	}

	if (child->IsMyChild(*this))
	{
		std::cout << "Components ancestor is his child" << std::endl;
		return false;
	}

	child->parent = this;
	children.push_back(std::unique_ptr<Component>(child));
	return true;
}

bool Component::IsMyChild(const Component& child) const
{
	if (children.size() < 1) return false;

	for (const auto& component : children)
	{
		if (component.get() == &child) return true;

		if (component->IsMyChild(child)) return true;
	}

	return false;
}

void Component::ReserveChildrenSize(int reserve) { children.reserve(reserve); }

#pragma endregion

#pragma region GettersSetters

std::string Component::GetName() const { return typeid(*this).name(); }
Component* Component::GetParent() const { return parent; }
const std::vector<std::unique_ptr<Component>>& Component::GetChildren() const { return children; }

const SDL_Rect* Component::GetAbsTf() const { return &absTf; }
SDL_Rect* Component::GetAbsTf() { return &absTf; }

#pragma endregion

bool Component::IsWithinBounds(int x, int y) const
{
	if (x < absTf.x) return false;
	if (y < absTf.y) return false;
	if (x > absTf.x + absTf.w) return false;
	if (y > absTf.y + absTf.h) return false;

	return true;
}