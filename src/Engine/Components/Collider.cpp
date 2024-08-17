#include "Collider.hpp"

Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::update(float deltaTime)
{
}
void Collider::physicsUpdate(float fixedDeltaTime)
{
}
void Collider::renderUpdate(float renderDeltaTime)
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

