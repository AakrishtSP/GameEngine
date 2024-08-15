#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <raylib.h>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include <type_traits>

#include "Base.hpp"

class Component; // Forward declaration of Component

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
    GameObject(const std::string &name) : name(name) {}

    std::string getName() const { return name; }

    template <typename T>
    std::shared_ptr<T> addComponent();
    std::shared_ptr<Component> addComponent(const std::string &type);
    std::shared_ptr<Component> addComponent(const std::string &type, const nlohmann::json &jsonData);

    template <typename T>
    std::shared_ptr<T> getComponent();

    void addChild(std::shared_ptr<GameObject> child);
    GameObject &addChild(const std::string &name);

    GameObject *getParent() const;

    std::vector<std::shared_ptr<GameObject>> getChildren() const { return children; }
    std::unordered_map<std::type_index, std::shared_ptr<Component>> getComponents() const { return components; }

    void update();

    virtual ~GameObject() {}

protected:
    std::string name;
    GameObject *parent = nullptr;
    std::vector<std::shared_ptr<GameObject>> children;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
};

template <typename T>
std::shared_ptr<T> GameObject::addComponent()
{
    auto component = std::make_shared<T>();
    component->setOwner(this);
    components[std::type_index(typeid(T))] = component;
    return component;
}

template <typename T>
std::shared_ptr<T> GameObject::getComponent()
{
    auto component = components.find(std::type_index(typeid(T)));
    if (component != components.end())
    {
        return std::dynamic_pointer_cast<T>(component->second);
    }
    return nullptr;
}
