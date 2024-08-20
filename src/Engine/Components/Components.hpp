#pragma once

#include "ComponentFactory.hpp"

#include "Transform2D.hpp"
#include "SpriteRenderer.hpp"
#include "Collider.hpp"
#include "RigidBody2D.hpp"
#include "ScriptLoader.hpp"

class ScriptLoader;

// Registering components
void inline registerComponents() {
    auto& factory = ComponentFactory::getInstance();
    factory.registerComponent<Transform2D>("Transform2D");
    factory.registerComponent<SpriteRenderer>("SpriteRenderer");
    factory.registerComponent<Collider>("Collider");
    factory.registerComponent<RigidBody2D>("RigidBody2D");
    factory.registerComponent<ScriptLoader>("ScriptLoader");
}
