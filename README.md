# The Loop (In Check In)

A card game built with **C++** and **SDL2**, designed with flexibility and maintainability in mind. This project features a custom UI framework, robust architecture and several useful systems for game development. 

<img width="780" height="400" alt="image" src="https://github.com/user-attachments/assets/5796103e-d4e4-4ab9-ac60-50f0e05c13f9" />
<img width="780" height="400" alt="image" src="https://github.com/user-attachments/assets/fdb8d297-1ce1-464e-8917-80478029782f" />

## 🚀 Features  

- **🎨 Easy-to-use UI Engine** – Simple to use UI syste
- **📏 Automatic Layout System** – Dynamically positions UI elements for a structured and responsive interface 
- **🗂️ Cache & Sound Manager** – Efficient asset handling to improve memory usage and resource management
- **🔗 Simple Entity & Component System** – Modular game objects with reusable components for flexibility
- **📚 Parent-Child GameObject System** – Uses **unique pointers** to prevent memory leaks and manage hierarchy efficiently
- **🃏 Pre-built Card Table Interface** – Initial game UI with a foundation for future gameplay mechanics

## Architecture

This project adheres to the OOP principles, several design patterns and modern C++ practices. This ensures that the game and underlying framework stays easy to maintain, flexible and modular. This is a compact showcase game, so I deliberately avoided over-abstraction. In a larger or team project, I would split systems further and introduce stronger decoupling. Showcases below are shorter for the sake of demonstration

### Singleton
Managers (Resource, Sound and Game) use a Singleton pattern to provide centralized access to resources, sound and game logic, ensuring consistent state across the engine: 
```cpp
template<typename Resource>
class ResourceManager
{
public:
    static ResourceManager& GetInstance()
    { 
        static ResourceManager atlas; // Meyers Singleton
        return atlas; 
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    ResourceManager() {}   // private constructor
    ~ResourceManager() { FreeAll(); }

    std::unordered_map<std::string, std::shared_ptr<Resource>> resourcePool;
};
```

### Composite Pattern
GameObject implements a composite pattern, allowing hierarchical object composition and recursive traversal, which is common in game engine design:
```cpp
private:
    std::vector<std::unique_ptr<GameObject>> children;

public:
    bool AdoptChild(std::unique_ptr<GameObject> child)
    {
        children.push_back(std::move(child));
    }

    void PrintFamilyTree(int spacing = 0)
    {
        std::cout << offset << GetName() << std::endl;
        for (const auto& component : children)
        {
            component->PrintFamilyTree(spacing + 1);
        }
    }
```

### Basic ECS  Pattern (Entity-Component-System) 
GameObject supports a component-based architecture, letting us attach behaviors dynamically, demonstrating understanding of modular and extensible design.

GameObject:
```cpp
private:
    std::vector<std::unique_ptr<Component>> components;

public:
    void AddComponent(Component* component)
    {
        components.push_back(std::unique_ptr<Component>(component));
    }
```
Component:
```cpp
class Component
{
  public:
  	Component() {}
   
  	virtual void Render(SDL_Surface* surface) = 0;
  	virtual void HandleEvent(const SDL_Event& event) = 0;
  
  	virtual ~Component() = default;
  
  protected:
  	GameObject* parent = nullptr;
};
```

### Observer
Uses an event system to decouple interconnected clasess with a function-based observers through lambdas. Ensures a smooth communication without coupling classes

GameManager:
```cpp
private:
    std::vector<std::function<void(int)>> onNewGame;

void CheckGoal()
{
    if (*measure > goal) // Winning the round and reseting the game
    {
        NewGame();
    }
}
```
Field:
```cpp
context.GM->AddOnNewGame([this] {
    EmptyField();
});
```
Hand:
```cpp
GM.AddOnNewGame([this, &deck]() {
    EmptyHand();

    AddCard(CardFactory::NewCard(deck.GetCard()));
    AddCard(CardFactory::NewCard(deck.GetCard()));
    AddCard(CardFactory::NewCard(deck.GetCard()));
});
```
