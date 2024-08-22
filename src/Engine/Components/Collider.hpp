#pragma once

#include "../Component.hpp"
#include "Transform2D.hpp"

class Transform2D;
class CollisionShape;


class Collider: public Component
{
public:
    Collider();
    ~Collider();

    nlohmann::json serialize() override;

    void deserialize(const nlohmann::json &j) override;

    void update(float deltatime) override;
    void renderUpdate(float renderDeltaTime) override{};
    void physicsUpdate(float fixedDeltaTime) override;

    std::shared_ptr<CollisionShape> addCollisionShape(const CollisionShape& shape);
    std::shared_ptr<CollisionShape> addCollisionShape(const Circle& circle, const Vector2& offset={0,0});
    std::shared_ptr<CollisionShape> addCollisionShape(const Rectangle& rectangle, const Vector2& offset={0,0}, float rotation=0);

    void removeCollisionShape(const std::shared_ptr<CollisionShape> &shape);

private:
    mutable std::shared_ptr<Transform2D> transform = nullptr;
    std::vector<std::shared_ptr<CollisionShape>> shapes;
};

class CollisionShape {
    bool isCircle;
    Circle circle;
    Rectangle rectangle;
    Vector2 offset;
    float rotation;
public:
    nlohmann::json serialize() ;
    void deserialize(const nlohmann::json &j);

    CollisionShape(const Circle& circle, const Vector2& offset={0,0}) : circle(circle), offset(offset), isCircle(true) {}
    CollisionShape(const Rectangle& rectangle, const Vector2& offset={0,0}, float rotation=0) : rectangle(rectangle), offset(offset), rotation(rotation), isCircle(false) {}
    CollisionShape() : isCircle(false), rectangle({0,0,0,0}), offset({0,0}), rotation(0) {};

    void setOffset(const Vector2& offset) { this->offset = offset; }
    void setRotation(float rotation) { this->rotation = rotation; }
    void setCircle(const Circle& circle) { this->circle = circle; isCircle = true; }
    void setCircle(const Circle& circle, const Vector2& offset) { this->circle = Circle{circle.center+offset,circle.radius}; isCircle = true; }
    void setCircle(const Vector2& center, float radius) { circle = Circle{center, radius}; isCircle = true; }
    void setCircle(const Vector2& center, float radius, const Vector2& offset) { circle = Circle{center+offset, radius}; isCircle = true; }

    void setRectangle(const Rectangle& rectangle) { this->rectangle = rectangle; isCircle = false; }
    void setRectangle(const Rectangle& rectangle, const Vector2& offset) { this->rectangle = rectangle; this->offset = offset; isCircle = false; }
    void setRectangle(const Rectangle& rectangle, const Vector2& offset, float rotation) { this->rectangle = rectangle; this->offset = offset; this->rotation = rotation; isCircle = false; }
    void setRectangle(float x, float y, float width, float height) { rectangle = Rectangle{x,y,width,height}; isCircle = false; }
    void setRectangle(float x, float y, float width, float height, const Vector2& offset) { rectangle = Rectangle{x,y,width,height}; this->offset = offset; isCircle = false; }
    void setRectangle(float x, float y, float width, float height, const Vector2& offset, float rotation) { rectangle = Rectangle{x,y,width,height}; this->offset = offset; this->rotation = rotation; isCircle = false; }

    Circle getCircle() const { return (isCircle) ? Circle{circle.center+offset,circle.radius} : Circle(); }
    Rect getRectangle() const { return (!isCircle) ? Rect{rectangle.x+offset.x,rectangle.y+offset.y,rectangle.width,rectangle.height, rotation}: Rect(); }

    operator Circle() const { return (isCircle) ? Circle{circle.center+offset,circle.radius} : Circle(); }
    operator Rect() const { return (!isCircle) ? Rect{rectangle.x+offset.x,rectangle.y+offset.y,rectangle.width,rectangle.height, rotation}: Rect(); }
};
