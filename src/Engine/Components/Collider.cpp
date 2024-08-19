#include "Collider.hpp"

Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::update(float deltatime)
{
}

void Collider::physicsUpdate(float fixedDeltaTime)
{
    
}

Vector2 Collider::supportFunction(Circle &circle, Vector2 &direction)
{
    Vector2 dir = Normalize(direction);
    return circle.center + dir * circle.radius;
}

Vector2 Collider::supportFunction(const Rectangle& rect, float rotation, const Vector2& direction) {

    Vector2 dir = Normalize(direction);
    Vector2 vertices[4]= {
        {rect.x, rect.y},
        {rect.x + rect.width, rect.y},
        {rect.x + rect.width, rect.y + rect.height},
        {rect.x, rect.y + rect.height}
    };
    float cosA = cos(rotation);
    float sinA = sin(rotation);
    
    // Rotate the vertices
    Vector2 rotationPoint = {rect.x + rect.width / 2, rect.y + rect.height / 2};
    for (int i = 0; i < 4; i++) {
        RotatePoint(vertices[i], rotationPoint, rotation);
        /*float x = vertices[i].x - rotationPoint.x;
        float y = vertices[i].y - rotationPoint.y;
        vertices[i].x = x * cosA - y * sinA + rotationPoint.x;
        vertices[i].y = x * sinA + y * cosA + rotationPoint.y; */
    }

    float maxDot = DotProduct(vertices[0], dir);
    Vector2 returnVertex = vertices[0];
    for (int i = 1; i < 4; i++) {
        float dot = DotProduct(vertices[i], dir);
        if (dot > maxDot) {
            maxDot = dot;
            returnVertex = vertices[i];
        }
    }

    return returnVertex;
}
