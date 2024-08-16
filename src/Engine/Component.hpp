#pragma once

#include "GameObject.hpp"

class GameObject;

class Component {
public:
    virtual ~Component() = default;
    virtual void update() = 0;
    void setOwner(GameObject* owner) { this->owner = owner; }
    GameObject* getOwner() const { return owner; }
    void setActive(bool active) { isActive = active; }


protected:
    bool isActive = true;
    GameObject* owner = nullptr;
};

