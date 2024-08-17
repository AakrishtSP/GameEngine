#include "GameObject.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "Component.hpp"
#include "Components/ComponentFactory.hpp"
#include "Vector2Ext.hpp"


void GameObject::addChild(std::shared_ptr<GameObject> child) {
    child->parent = this;
    children.push_back(child);
}

GameObject &GameObject::addChild(const std::string &name) {
    auto child = std::make_shared<GameObject>(name);
    addChild(child);
    return *child;
}

GameObject *GameObject::getParent() const { return parent; }

nlohmann::json GameObject::serialize() {
    nlohmann::json json;
    json["name"] = name;
    json["components"] = nlohmann::json::array();
    for (auto it = components.begin(); it != components.end();) {
        if (it->second) {
            json["components"].push_back(it->second->serialize());
            ++it;
        } else {
            // Handle case where component has been removed or is invalid
            it = components.erase(it);
        }
    }
    json["children"] = nlohmann::json::array();
    for (auto it = children.begin(); it != children.end();) {
        if (*it) {
            json["children"].push_back((*it)->serialize());
            ++it;
        } else {
            it = children.erase(it);
        }
    }
    return json;
}

void GameObject::deserialize(const nlohmann::json &json) {
    name = json["name"];
    components.clear();
    // for (const auto& jsonComponent : json["components"])
    // {
    //     std::string type = jsonComponent["componentType"];
    //     std::shared_ptr<Component> component = ComponentFactory::getInstance().create(type);

    //     if (component)
    //     {
    //         component->setOwner(this);
    //         component->deserialize(jsonComponent);
    //         components[std::type_index(typeid(*component))] = component;
    //     }
    //     else
    //     {
    //         std::cerr << "Failed to create component of type: " << type << std::endl;
    //     }
    // }
    for (auto it = json["components"].rbegin(); it != json["components"].rend(); ++it) {
        const auto &jsonComponent = *it;
        std::string type = jsonComponent["componentType"];
        auto component = ComponentFactory::getInstance().create(type);

        if (component) {
            component->setOwner(this);
            component->deserialize(jsonComponent);
            components[std::type_index(typeid(*component))] = component;
        } else {
            std::cerr << "Failed to create component of type: " << type << std::endl;
        }
    }

    children.clear();
    for (const auto &jsonChild: json["children"]) {
        auto child = std::make_shared<GameObject>("");
        child->deserialize(jsonChild);
        addChild(child);
    }
}

void GameObject::update(float deltaTime) {
    // Iterate over the original components
    for (auto it = components.begin(); it != components.end();) {
        if (it->second) {
            it->second->update(deltaTime);
            ++it;
        } else {
            // Handle case where component has been removed or is invalid
            it = components.erase(it);
        }
    }

    // Iterate over the original children
    for (auto it = children.begin(); it != children.end();) {
        if (*it) {
            (*it)->update(deltaTime);
            ++it;
        } else {
            it = children.erase(it);
        }
    }
}

void GameObject::physicsUpdate(float fixedDeltaTime) {
    // Iterate over the original components
    for (auto it = components.begin(); it != components.end();) {
        if (it->second) {
            it->second->physicsUpdate(fixedDeltaTime);
            ++it;
        } else {
            // Handle case where component has been removed or is invalid
            it = components.erase(it);
        }
    }

    // Iterate over the original children
    for (auto it = children.begin(); it != children.end();) {
        if (*it) {
            (*it)->physicsUpdate(fixedDeltaTime);
            ++it;
        } else {
            it = children.erase(it);
        }
    }
}

void GameObject::renderUpdate(float renderDeltaTime) {
    // Iterate over the original components
    for (auto it = components.begin(); it != components.end();) {
        if (it->second) {
            it->second->renderUpdate(renderDeltaTime);
            ++it;
        } else {
            // Handle case where component has been removed or is invalid
            it = components.erase(it);
        }
    }

    // Iterate over the original children
    for (auto it = children.begin(); it != children.end();) {
        if (*it) {
            (*it)->renderUpdate(renderDeltaTime);
            ++it;
        } else {
            it = children.erase(it);
        }
    }
}

GameObject::~GameObject() {
    // Iterate over the original components
    for (auto it = components.begin(); it != components.end();) {
        if (it->second) {
            it->second.reset();
            ++it;
        } else {
            // Handle case where component has been removed or is invalid
            it = components.erase(it);
        }
    }

    // Iterate over the original children
    for (auto it = children.begin(); it != children.end();) {
        if (*it) {
            (*it).reset();
            ++it;
        } else {
            it = children.erase(it);
        }
    }
}
