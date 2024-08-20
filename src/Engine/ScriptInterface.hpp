#pragma once

#include "Base.hpp"

class ScriptInterface {
public:
    virtual ~ScriptInterface() = default;

    virtual void setOwner(GameObject* owner) = 0;

    virtual void onStart() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onPhysicsUpdate(float fixedDeltaTime) = 0;
};
