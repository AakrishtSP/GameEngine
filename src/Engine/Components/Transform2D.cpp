#include "Transform2D.hpp"
#include "../Component.hpp"


nlohmann::json Transform2D::serialize() { 
    nlohmann::json json;
    json["componentType"] = "Transform2D";
    json["isActive"] = isActive;
    json["position"] = {position.x, position.y};
    json["rotation"] = rotation;
    json["scale"] = scale;
    return json;
}

void Transform2D::deserialize(const nlohmann::json &json) {
    isActive = json["isActive"].get<bool>();

    position.x = json["position"][0].get<float>();
    position.y = json["position"][1].get<float>();

    rotation = json["rotation"].get<float>();

    scale = json["scale"].get<float>();
}

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


void Transform2D::update(float deltaTime) {
    calculateWorldPosition();
}

Vector2 Transform2D::getGamePosition() const {
    // Get the half of the screen width and height
    float halfScreenWidth = GetScreenWidth() / 2.0f;
    float halfScreenHeight = GetScreenHeight() / 2.0f;

    // Calculate the game position with flipped y axis
    float gamePosX = halfScreenWidth + worldPosition.x;
    float gamePosY = halfScreenHeight - worldPosition.y;

    return Vector2{gamePosX, gamePosY};
}
