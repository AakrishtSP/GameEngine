#pragma once

#include <cfloat>
#include "../Component.hpp"
#include "Collider.hpp"
#include "Transform2D.hpp"
#include "raylib.h"


class RigidBody2D : public Component {
protected:
    std::shared_ptr<Transform2D> transform = nullptr;
    std::shared_ptr<Collider> collider = nullptr;
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 force;
    float mass;
    bool isMoveable;
    float affectedByGravity;
    Vector2 gravity;
    Vector2 totalForce;

public:
    RigidBody2D();
    ~RigidBody2D() override = default;

    nlohmann::json serialize() override;
    void deserialize(const nlohmann::json &json) override;

    void update(float deltaTime) override;
    void physicsUpdate(float fixedDeltaTime) override;
    void renderUpdate(float renderDeltaTime) override {};

    Rectangle drawInspector(Rectangle &rectangle) override;

    void setMass(const float mass) { this->mass = mass; }
    void setMoveable(const bool isMoveable) { this->isMoveable = isMoveable; }
    void setVelocity(const Vector2 &velocity) { this->velocity = velocity; }
    void setVelocityX(const float x) { this->velocity.x = x; }
    void setVelocityY(const float y) { this->velocity.y = y; }
    void setAcceleration(const Vector2 &acceleration) { this->acceleration = acceleration; }
    void setForce(const Vector2 &force) { this->force = force; }
    void addForce(const Vector2 &force) { this->force = this->force + force; }
    void setAffectedByGravity(const float affectedByGravity);

    [[nodiscard]] Vector2 getGravity() const { return gravity; }
    [[nodiscard]] float getAffectedByGravity() const { return affectedByGravity; }

    [[nodiscard]] Vector2 getVelocity() const { return velocity; }
    [[nodiscard]] Vector2 getAcceleration() const { return acceleration; }
    [[nodiscard]] Vector2 getForce() const { return force; }
    [[nodiscard]] float getMass() const { return mass; }
    [[nodiscard]] bool getMoveable() const { return isMoveable; }

    void getTransform();
    void getCollider();
};
