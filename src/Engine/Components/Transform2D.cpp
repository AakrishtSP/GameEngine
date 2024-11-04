#include "Transform2D.hpp"
#include "../Component.hpp"
#include "raygui.h"

Transform2D::Transform2D() : position({0, 0}), rotation(0), scale(1), worldScale(1), worldPosition(), worldRotation(0) {
    name = "Transform2D";
    editorEditMode.resize(4, 0);
}
Rect Transform2D::ToGameRect(const Rect &rect) {

    // Get the half of the screen width and height
    float halfScreenWidth = GetScreenWidth() / 2.0f;
    float halfScreenHeight = GetScreenHeight() / 2.0f;

    // Calculate the game position with flipped y axis
    float gamePosX = halfScreenWidth + rect.x;
    float gamePosY = halfScreenHeight - rect.y;
    float gameWidth = rect.width;
    float gameHeight = rect.height;

    return Rect{gamePosX, gamePosY, gameWidth, gameHeight};
}

nlohmann::json Transform2D::serialize() {
    nlohmann::json json;
    json["componentType"] = name;
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
    // std::cout << owner->getName() << " : " << position.x << ", " << position.y << std::endl;
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

Vector2 Transform2D::getGamePosition() const {
    // Get the half of the screen width and height
    float halfScreenWidth = GetScreenWidth() / 2.0f;
    float halfScreenHeight = GetScreenHeight() / 2.0f;

    // Calculate the game position with flipped y axis
    float gamePosX = halfScreenWidth + worldPosition.x;
    float gamePosY = halfScreenHeight - worldPosition.y;

    return Vector2{gamePosX, gamePosY};
}

Vector2 Transform2D::getWorldPosition() {
    calculateWorldPosition();
    return worldPosition;
}

float Transform2D::getWorldRotation() {
    calculateWorldPosition();
    return worldRotation;
}

float Transform2D::getWorldScale() {
    calculateWorldPosition();
    return worldScale;
}

Rectangle Transform2D::drawInspector(Rectangle &previousRectangle) {
    // Define the width of the group box and padding
    float padding = 10.0f;
    float groupBoxWidth = previousRectangle.width - 2 * padding;

    // Initialize the height to accommodate the title of the group box
    float groupBoxHeight = padding;
    float currentY = previousRectangle.y + previousRectangle.height + padding;

    // Calculate positions for the first label and text boxes
    float labelX = previousRectangle.x + padding;
    float labelWidth = previousRectangle.width / 3.0f;
    float textBoxX = labelX + labelWidth + 10.0f;
    float textBoxWidth = groupBoxWidth - labelWidth - 3 * padding;

    // Draw and update height for each UI element
    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Position");
    editorEditMode.at(0) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                                      &std::to_string(position.x)[0u], 10, editorEditMode[0]); // x
    editorEditMode.at(1) =
            GuiTextBox(Rectangle{textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                       &std::to_string(position.y)[0u], 10, editorEditMode[1]); // y
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Rotation");
    editorEditMode.at(2) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth, 20}, &std::to_string(rotation)[0u],
                                      10, editorEditMode[2]); // rotation
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Scale");
    editorEditMode.at(3) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth - 5.0f, 20},
                                      &std::to_string(scale)[0u], 10, editorEditMode[3]); // scale
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    // Final group box position and size
    Rectangle groupBoxRect = {
            previousRectangle.x + padding, previousRectangle.y + previousRectangle.height + padding, groupBoxWidth,
            groupBoxHeight + padding // Add some padding at the bottom
    };

    // Draw the group box with the "Transform2D" label
    GuiGroupBox(groupBoxRect, "Transform2D");

    Rectangle returnRect = {previousRectangle.x, previousRectangle.y + previousRectangle.height,
                            previousRectangle.width, groupBoxHeight + padding * 5};

    // Return the bounding rectangle for the entire group box
    return returnRect;
}

