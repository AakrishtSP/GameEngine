#pragma once

#include "GameObject.hpp"

class GameObject;

class Component {
public:
    virtual ~Component() = default;
    virtual void update() = 0;
    void setOwner(GameObject* owner) { this->owner = owner; }
    [[nodiscard]] GameObject* getOwner() const { return owner; }
    void setActive(const bool active) { isActive = active; }

    [[nodiscard]] std::string getType() const { return type; }


protected:
    std::string type;
    bool isActive = true;
    GameObject* owner = nullptr;
};

