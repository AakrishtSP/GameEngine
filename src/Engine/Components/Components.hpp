#pragma once

#include "ComponentFactory.hpp"

#include "Transform2D.hpp"
#include "SpriteRenderer.hpp"
// #include "Collider.hpp"

// Registering components
void registerComponents() {
    auto& factory = ComponentFactory::getInstance();
    factory.registerComponent<Transform2D>("Transform2D");
    factory.registerComponent<SpriteRenderer>("SpriteRenderer");
    // Add more components as needed
}
