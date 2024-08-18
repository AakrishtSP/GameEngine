#pragma once

#include "GameObject.hpp"

class GameObject;

class Component {
public:
    virtual ~Component() = default;
    virtual void update(float deltaTime) = 0;
    virtual void physicsUpdate(float fixedDeltaTime) {}
    virtual void renderUpdate(float renderDeltaTime) {}
    void setOwner(GameObject* owner) { this->owner = owner; }
    GameObject* getOwner() const { return owner; }
    void setActive(bool active) { isActive = active; }

    virtual Rectangle drawInspector(Rectangle& Rectangle) {return Rectangle;}

    virtual nlohmann::json serialize() {
        nlohmann::json json;
        json["componentType"] = name;
        json["isActive"] = isActive;
        return json;
    }

    virtual void deserialize(const nlohmann::json& json) {
        isActive = json["isActive"].get<bool>();
    }

    std::string getName() const { return name; }


protected:
    bool isActive = true;
    std::string name = "Component";
    GameObject* owner = nullptr;
};