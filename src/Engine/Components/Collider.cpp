#include "Collider.hpp"

Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::update()
{
}

Vector2 Collider::support(Circle &circle, Vector2 &direction)
{
    Vector2 dir = Normalize(direction);
    return circle.center + dir * circle.radius;
}

Vector2 Collider::support(Rectangle &rectangle, Vector2 &direction)
{
    Vector2 dir = Normalize(direction);

    return Vector2();
}

Vector2 Collider::support(Polygon &polygon, Vector2 &direction)
{
    Vector2 dir = Normalize(direction);

    return Vector2();
}

