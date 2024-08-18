#include "RigidBody2D.hpp"

nlohmann::json RigidBody2D::serialize()
{
    nlohmann::json json;
    json["componentType"] = "RigidBody2D";
    json["isActive"] = isActive;
    json["velocity"] = {velocity.x, velocity.y};
    json["acceleration"] = {acceleration.x, acceleration.y};
    json["force"] = {force.x, force.y};
    return json;
}

void RigidBody2D::deserialize(const nlohmann::json& json)
{
    isActive = json["isActive"].get<bool>();

    velocity.x = json["velocity"][0].get<float>();
    velocity.y = json["velocity"][1].get<float>();

    acceleration.x = json["acceleration"][0].get<float>();
    acceleration.y = json["acceleration"][1].get<float>();

    force.x = json["force"][0].get<float>();
    force.y = json["force"][1].get<float>();
}

void RigidBody2D::update(float deltaTime)
{
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;

    Vector2 currentPosition = transform->getPosition();
    currentPosition.x += velocity.x * deltaTime;
    currentPosition.y += velocity.y * deltaTime;

    velocity.x += (force.x / mass) * deltaTime;
    velocity.y += (force.y / mass) * deltaTime;

    transform->setPosition(currentPosition);

    force.x = 0.0f;
    force.y = 0.0f;
}

void RigidBody2D::physicsUpdate(float fixedDeltaTime)
{
    velocity.x += acceleration.x * fixedDeltaTime;
    velocity.y += acceleration.y * fixedDeltaTime;

    Vector2 gravity = {0, -9.8f};
    velocity.x += gravity.x * fixedDeltaTime;
    velocity.y += gravity.y * fixedDeltaTime;
    
    velocity.x += (force.x / mass) * fixedDeltaTime;
    velocity.y += (force.y / mass) * fixedDeltaTime;

    Vector2 currentPosition = transform->getPosition();
    currentPosition.x += velocity.x * fixedDeltaTime;
    currentPosition.y += velocity.y * fixedDeltaTime;

    velocity.x += (force.x / mass) * fixedDeltaTime;
    velocity.y += (force.y / mass) * fixedDeltaTime;

    transform->setPosition(currentPosition);

    force.x = 0.0f;
    force.y = 0.0f;
}

void RigidBody2D::getTransform()
{
    if (!owner) {
        std::cerr << "owner not found" << std::endl;
        return;
    }
    transform = owner->getComponent<Transform2D>();
    if (!transform) {
        transform = owner->addComponent<Transform2D>();
    }
}

void RigidBody2D::getCollider()
{
    if (!owner) {
        std::cerr << "owner not found" << std::endl;
        return;
    }
    collider = owner->getComponent<Collider>();
    if (!collider) {
        collider = owner->addComponent<Collider>();
    }
}