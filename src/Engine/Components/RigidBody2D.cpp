#include "RigidBody2D.hpp"
#include "../GameEngine.hpp"
#include "raygui.h"


RigidBody2D::RigidBody2D() :
    velocity({0, 0}), acceleration({0, 0}), force({0, 0}), isMoveable(true), mass(1), affectedByGravity(1),
    gravity({0, 0}) {
    name = "RigidBody2D";
    editorEditMode.resize(7, 0);
}


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
    if (isMoveable) {
        // Get Gravity
        gravity = GameEngine::getInstance().getWorldGravity() * affectedByGravity;
        // Calculate total force
        totalForce = force + gravity * mass;

        // Apply forces
        position = transform->getPosition();
        position = 2 * position - previousPosition + totalForce / mass * fixedDeltaTime * fixedDeltaTime;

        previousPosition = transform->getPosition();
    }

    // Update position (assuming you have a way to update position)
    if (!transform) {
        getTransform();
    } else {
        position = position + velocity * fixedDeltaTime;
        transform->setPosition(position);
    }
    // Clear force
    force = {0, 0};
    totalForce = {0, 0};
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

    // Define persistent strings to hold the values
    std::string velocityX = std::to_string(velocity.x);
    std::string velocityY = std::to_string(velocity.y);
    std::string accelerationX = std::to_string(acceleration.x);
    std::string accelerationY = std::to_string(acceleration.y);
    std::string forceX = std::to_string(force.x);
    std::string forceY = std::to_string(force.y);
    std::string massStr = std::to_string(mass);

    // Create buffers to use with GuiTextBox
    char velocityXBuffer[64], velocityYBuffer[64];
    char accelerationXBuffer[64], accelerationYBuffer[64];
    char forceXBuffer[64], forceYBuffer[64];
    char massBuffer[64];

    // Copy the string content to the buffers
    strncpy(velocityXBuffer, velocityX.c_str(), sizeof(velocityXBuffer));
    strncpy(velocityYBuffer, velocityY.c_str(), sizeof(velocityYBuffer));
    strncpy(accelerationXBuffer, accelerationX.c_str(), sizeof(accelerationXBuffer));
    strncpy(accelerationYBuffer, accelerationY.c_str(), sizeof(accelerationYBuffer));
    strncpy(forceXBuffer, forceX.c_str(), sizeof(forceXBuffer));
    strncpy(forceYBuffer, forceY.c_str(), sizeof(forceYBuffer));
    strncpy(massBuffer, massStr.c_str(), sizeof(massBuffer));

    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Velocity");
    editorEditMode.at(0) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20}, velocityXBuffer,
                                      sizeof(velocityXBuffer), editorEditMode[0]); // x
    editorEditMode.at(1) =
            GuiTextBox(Rectangle{textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                       velocityYBuffer, sizeof(velocityYBuffer), editorEditMode[1]); // y
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Acceleration");
    editorEditMode.at(2) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                                      accelerationXBuffer, sizeof(accelerationXBuffer), editorEditMode[2]); // x
    editorEditMode.at(3) =
            GuiTextBox(Rectangle{textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                       accelerationYBuffer, sizeof(accelerationYBuffer), editorEditMode[3]); // y
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Force");
    editorEditMode.at(4) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20}, forceXBuffer,
                                      sizeof(forceXBuffer), editorEditMode[4]); // x
    editorEditMode.at(5) =
            GuiTextBox(Rectangle{textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                       forceYBuffer, sizeof(forceYBuffer), editorEditMode[5]); // y
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Mass");
    editorEditMode.at(6) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth, 20}, massBuffer, sizeof(massBuffer),
                                      editorEditMode[6]); // mass
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    // Optionally: Convert the buffers back to floats or other types if needed
    velocity.x = std::stof(velocityXBuffer);
    velocity.y = std::stof(velocityYBuffer);
    acceleration.x = std::stof(accelerationXBuffer);
    acceleration.y = std::stof(accelerationYBuffer);
    force.x = std::stof(forceXBuffer);
    force.y = std::stof(forceYBuffer);
    mass = std::stof(massBuffer);


    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Moveable");
    GuiCheckBox(Rectangle{textBoxX, currentY, 20, 20}, "", &isMoveable); // isMoveable
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    // Final group box position and size
    Rectangle groupBoxRect = {
            previousRectangle.x + padding, previousRectangle.y + previousRectangle.height + padding, groupBoxWidth,
            groupBoxHeight + padding // Add some padding at the bottom
    };

    // Draw the group box with the "RigidBody2D" label
    GuiGroupBox(groupBoxRect, "RigidBody2D");

    Rectangle returnRect = {previousRectangle.x, previousRectangle.y + previousRectangle.height,
                            previousRectangle.width, groupBoxHeight + padding * 5};


    // Return the bounding rectangle for the entire group box
    return returnRect;
}

void RigidBody2D::setAffectedByGravity(const float affectedByGravity) { this->affectedByGravity = affectedByGravity; }
