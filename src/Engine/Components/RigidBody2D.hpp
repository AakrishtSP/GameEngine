#pragma once

#include<cfloat>
#include "raylib.h"
#include "Transform2D.hpp"
#include "Collider.hpp"
#include "../Component.hpp"

class RigidBody2D : public Component
{
protected:
    std::shared_ptr<Transform2D> transform = nullptr;
    std::shared_ptr<Collider> collider = nullptr;
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 force;
    float mass;
    bool isMoveable;

public:
    RigidBody2D(bool isMoveable=true) : velocity({0, 0}), acceleration({0, 0}), force({0, 0}), isMoveable(isMoveable) {
        if(!isMoveable){
            mass = FLT_MAX;
        }
    }
    ~RigidBody2D() override = default;

    nlohmann::json serialize() override;
    void deserialize(const nlohmann::json& json) override;

    void update(float deltaTime) override;
    void physicsUpdate(float fixedDeltaTime) override;
    void renderUpdate(float renderDeltaTime) override{};

    void setVelocity(const Vector2& velocity) { this->velocity = velocity; }
    void setAcceleration(const Vector2& acceleration) { this->acceleration = acceleration; }
    void setForce(const Vector2& force) { this->force = force; }
    void setMass(const float mass){
        if(isMoveable){
            this->mass = mass;
        }this->mass = FLT_MAX;
    }
    void addForce(const Vector2& force) { this->force = this->force + force; }
    [[nodiscard]] Vector2 getVelocity() const { return velocity; }
    [[nodiscard]] Vector2 getAcceleration() const { return acceleration; }

    void getTransform();
    void getCollider();
};
