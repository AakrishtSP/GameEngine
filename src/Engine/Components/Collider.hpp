#pragma once

#include "../Component.hpp"
#include "Transform2D.hpp"

class Transform2D;



class Collider: public Component
{
public:
    Collider();
    ~Collider();

    void update(float deltatime) override;
    void renderUpdate(float renderDeltaTime) override{};
    void physicsUpdate(float fixedDeltaTime) override;
    void renderUpdate(float renderDeltaTime) override;
    Vector2 support(Circle &circle, Vector2 &direction);
    Vector2 support(Rectangle &rectangle, Vector2 &direction);
    Vector2 support(Polygon &polygon, Vector2 &direction);
    void update() override;
    Vector2 supportFunction(Circle &circle, Vector2 &direction);
    Vector2 supportFunction(const Rectangle& rect, float rotation, const Vector2& direction);
    //Vector2 support(Rectangle &rectangle, Vector2 &direction);
    //Vector2 support(Polygon &polygon, Vector2 &direction);
    Vector2 supportFunction(Circle &circle, Vector2 &direction);
    Vector2 supportFunction(const Rectangle& rect, float rotation, const Vector2& direction);

private:
    mutable std::shared_ptr<Transform2D> transform = nullptr;
    Rectangle rectangle;
    Vector2 offset;

};
