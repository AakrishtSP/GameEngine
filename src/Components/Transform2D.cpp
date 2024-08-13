#include "Transform2D.hpp"
#include "../Component.hpp"

inline void Transform2D::transferOrigin() {
    gamePosition = {(GetScreenWidth() / 2) + worldPosition.x, (GetScreenHeight() / 2) - worldPosition.y};
}

inline void Transform2D::calculateWorldPosition() {
    if (owner == nullptr) {
        std::cerr << "owner not found" << std::endl;
        return;
    }
    auto parentObject = owner->getParent();
    if (parentObject == nullptr) {
        worldPosition = position;
        worldRotation = rotation;
        worldScale = scale;
        return;
    } else {
        auto parentTransform = parentObject->getComponent<Transform2D>();
        if (parentTransform) {
            worldPosition.x = parentTransform->worldPosition.x + position.x;
            worldPosition.y = parentTransform->worldPosition.y + position.y;
            worldRotation = parentTransform->worldRotation + rotation;
            worldScale = parentTransform->worldScale * scale;
        }
    }
}


void Transform2D::update() {
    calculateWorldPosition();
    transferOrigin();
    // std::cout << "World Position: " << worldPosition.x << ", " << worldPosition.y << std::endl;
    // std::cout << "World Rotation: " << worldRotation << std::endl;
}
