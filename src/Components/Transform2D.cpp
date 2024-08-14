#include "Transform2D.hpp"
#include "../Component.hpp"


inline void Transform2D::calculateWorldPosition() {
    if (owner == nullptr) {
        std::cerr << "owner not found" << std::endl;
        return;
    }
    if (const auto parentObject = owner->getParent(); parentObject == nullptr) {
        worldPosition = position;
        worldRotation = rotation;
        worldScale = scale;
        return;
    } else {
        if (const auto parentTransform = parentObject->getComponent<Transform2D>()) {
            worldPosition.x = parentTransform->worldPosition.x + position.x;
            worldPosition.y = parentTransform->worldPosition.y + position.y;
            worldRotation = parentTransform->worldRotation + rotation;
            worldScale = parentTransform->worldScale * scale;
        }
    }
}


void Transform2D::update() {
    calculateWorldPosition();
    // std::cout << "World Position: " << worldPosition.x << ", " << worldPosition.y << std::endl;
    // std::cout << "World Rotation: " << worldRotation << std::endl;
}
