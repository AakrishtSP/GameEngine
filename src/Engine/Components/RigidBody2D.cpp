#include "RigidBody2D.hpp"

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
