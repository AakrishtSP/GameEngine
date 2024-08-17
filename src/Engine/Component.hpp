#pragma once

#include "GameObject.hpp"

class GameObject;

class Component {
public:
    virtual ~Component() = default;
    virtual void update(float deltaTime) = 0;
    void setOwner(GameObject* owner) { this->owner = owner; }
    GameObject* getOwner() const { return owner; }
    void setActive(bool active) { isActive = active; }

    virtual nlohmann::json serialize() {
        nlohmann::json json;
        json["componentType"] = "Component";
        json["isActive"] = isActive;
        return json;
    }

    virtual void deserialize(const nlohmann::json& json) {
        isActive = json["isActive"].get<bool>();
    }


protected:
    bool isActive = true;
    GameObject* owner = nullptr;
};