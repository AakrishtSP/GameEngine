#include "RigidBody2D.hpp"
#include "raygui.h"

nlohmann::json RigidBody2D::serialize() {
    nlohmann::json json;
    json["componentType"] = "RigidBody2D";
    json["isActive"] = isActive;
    json["velocity"] = {velocity.x, velocity.y};
    json["acceleration"] = {acceleration.x, acceleration.y};
    json["force"] = {force.x, force.y};
    json["mass"] = mass;
    json["isMoveable"] = isMoveable;
    return json;
}

void RigidBody2D::deserialize(const nlohmann::json &json) {
    isActive = json["isActive"].get<bool>();

    velocity.x = json["velocity"][0].get<float>();
    velocity.y = json["velocity"][1].get<float>();

    acceleration.x = json["acceleration"][0].get<float>();
    acceleration.y = json["acceleration"][1].get<float>();

    force.x = json["force"][0].get<float>();
    force.y = json["force"][1].get<float>();

    mass = json["mass"].get<float>();
    isMoveable = json["isMoveable"].get<bool>();
}

void RigidBody2D::update(float deltaTime) {}

void RigidBody2D::physicsUpdate(float fixedDeltaTime) {
    if (!isMoveable) return;

    // Apply forces
    Vector2 acceleration = force / mass;
    velocity = velocity + acceleration * fixedDeltaTime;

    // Update position (assuming you have a way to update position)
    if (transform) {
        Vector2 position = transform->getPosition();
        position = position + velocity * fixedDeltaTime;
        transform->setPosition(position);
    }

    // Clear force
    force = {0, 0};
}


void RigidBody2D::getTransform() {
    if (!owner) {
        std::cerr << "owner not found" << std::endl;
        return;
    }
    transform = owner->getComponent<Transform2D>();
    if (!transform) {
        transform = owner->addComponent<Transform2D>();
    }
}

void RigidBody2D::getCollider() {
    if (!owner) {
        std::cerr << "owner not found" << std::endl;
        return;
    }
    collider = owner->getComponent<Collider>();
    if (!collider) {
        collider = owner->addComponent<Collider>();
    }
}


Rectangle RigidBody2D::drawInspector(Rectangle &previousRectangle) {
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

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Velocity");
    GuiTextBox(Rectangle{ textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20 }, &std::to_string(velocity.x)[0u], 10, true); // x
    GuiTextBox(Rectangle{ textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20 }, &std::to_string(velocity.y)[0u], 10, true); // y
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Acceleration");
    GuiTextBox(Rectangle{ textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20 }, &std::to_string(acceleration.x)[0u], 10, true); // x
    GuiTextBox(Rectangle{ textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20 }, &std::to_string(acceleration.y)[0u], 10, true); // y
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Force");
    GuiTextBox(Rectangle{ textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20 }, &std::to_string(force.x)[0u], 10, true); // x
    GuiTextBox(Rectangle{ textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20 }, &std::to_string(force.y)[0u], 10, true); // y
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Mass");
    GuiTextBox(Rectangle{ textBoxX, currentY, textBoxWidth, 20 }, &std::to_string(mass)[0u], 10, true); // mass
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Moveable");
    GuiCheckBox(Rectangle{ textBoxX, currentY, 20, 20 }, "", &isMoveable); // isMoveable
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    // Final group box position and size
    Rectangle groupBoxRect = {
        previousRectangle.x + padding,
        previousRectangle.y + previousRectangle.height + padding,
        groupBoxWidth,
        groupBoxHeight + padding // Add some padding at the bottom
    };

    // Draw the group box with the "RigidBody2D" label
    GuiGroupBox(groupBoxRect, "RigidBody2D");

    Rectangle returnRect = {
        previousRectangle.x,
        previousRectangle.y,
        previousRectangle.width,
        groupBoxHeight + padding * 5
    };

    // Return the bounding rectangle for the entire group box
    return returnRect;
}
