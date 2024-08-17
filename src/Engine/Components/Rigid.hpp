#pragma once
#include "raylib.h"
#include "Transform2D.hpp"
#include "SpriteRenderer.hpp"
#include "../Component.hpp"

class RigidBody
{
protected:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    double mass;
    double width;
    double height;

public:
    RigidBody(double m, double w, double h): mass(m), width(w), height(h){
        position = {0.0f, 0.0f};
        velocity = {0.0f, 0.0f};
        acceleration = {0.0f, 0.0f};
    }
    Vector2 getpos()const {return position;}
    Vector2 getvel()const {return velocity;}
    Vector2 getacc()const {return acceleration;}
    double getmass()const {return mass;}

    void setPos(Vector2 pos){position = pos;}
    void setVel(Vector2 vel){velocity = vel;}
    void setAcc(Vector2 acc){acceleration = acc;}
};

void UpdateRigidBody(RigidBody &body, double dt){
    Vector2 newVelocity = body.getvel();
    newVelocity.x += body.getacc().x * dt;
    newVelocity.y += body.getacc().y * dt;
    body.setVel(newVelocity);

    Vector2 newPosition = body.getpos();
    newPosition.x += body.getpos().x * dt;
    newPosition.y += body.getpos().y * dt;
    body.setPos(newPosition);

    body.setAcc({0, 0});
}

void applyForce(RigidBody &body, Vector2 force){
    Vector2 newAcceleration = body.getacc();
    newAcceleration.x += force.x / body.getmass();
    newAcceleration.y += force.y / body.getmass();
}

