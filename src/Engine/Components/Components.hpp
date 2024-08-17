#pragma once

#include "ComponentFactory.hpp"

#include "Transform2D.hpp"
#include "SpriteRenderer.hpp"
#include "Collider.hpp"
#include "RigidBody2D.hpp"

// Registering components
void registerComponents() {
    auto& factory = ComponentFactory::getInstance();
    factory.registerComponent<Transform2D>("Transform2D");
    factory.registerComponent<SpriteRenderer>("SpriteRenderer");
    factory.registerComponent<Collider>("Collider");
    factory.registerComponent<RigidBody2D>("RigidBody2D");
}
