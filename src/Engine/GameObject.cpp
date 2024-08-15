#include "GameObject.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <vector>
#include "Component.hpp"
#include "Vector2Ext.hpp"


std::shared_ptr<Component> GameObject::addComponent(const std::string &type) { 
    return std::shared_ptr<Component>();
 }

std::shared_ptr<Component> GameObject::addComponent(const std::string &type, const nlohmann::json &jsonData) {
    return std::shared_ptr<Component>();
}

void GameObject::addChild(std::shared_ptr<GameObject> child) {
    child->parent = this;
    children.push_back(child);
}

GameObject &GameObject::addChild(const std::string &name) {
    auto child = std::make_shared<GameObject>(name);
    addChild(child);
    return *child;
}

GameObject *GameObject::getParent() const {
    return parent;
}

void GameObject::update() {
    // Iterate over the original components
    for (auto it = components.begin(); it != components.end();) {
        if (it->second) {
            it->second->update();
            ++it;
        } else {
            // Handle case where component has been removed or is invalid
            it = components.erase(it);
        }
    }

    // Iterate over the original children
    for (auto it = children.begin(); it != children.end();) {
        if (*it) {
            (*it)->update();
            ++it;
        } else {
            it = children.erase(it);
        }
    }
}
