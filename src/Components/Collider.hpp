#pragma once

#include "../Component.hpp"
#include "Transform2D.hpp"

class Transform2D;



class Collider: public Component
{
public:
    Collider();
    ~Collider();

    void update() override;
    Vector2 support(Circle &circle, Vector2 &direction);
    Vector2 support(Rectangle &rectangle, Vector2 &direction);
    Vector2 support(Polygon &polygon, Vector2 &direction);

private:
    mutable std::shared_ptr<Transform2D> transform = nullptr;
    Rectangle rectangle;
    Vector2 offset;

};
