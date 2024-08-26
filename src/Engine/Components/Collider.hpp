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

    std::vector<std::shared_ptr<CollisionShape>> getShapes() const { return shapes; }

    Rectangle drawInspector(Rectangle& rectangle) override;

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
    // std::vector<int> layer;
    // std::vector<int> mask;
    std::vector<int> editorEditMode;
public:
    nlohmann::json serialize() ;
    void deserialize(const nlohmann::json &j);

    Rectangle drawInspector(Rectangle& rectangle);

    CollisionShape(const Circle& circle, const Vector2& offset);
    CollisionShape(const Rectangle& rectangle, const Vector2& offset, float rotation);
    CollisionShape();

    void setOffset(const Vector2& offset) { this->offset = offset; }
    void setRotation(float rotation) { this->rotation = rotation; }
    void setCircle(const Circle& circle, const Vector2& offset={0,0}) { this->circle = Circle{circle.center+offset,circle.radius}; isCircle = true; }
    void setCircle(const Vector2& center, float radius, const Vector2& offset={0,0}) { circle = Circle{center+offset, radius}; isCircle = true; }

    void setRectangle(const Rectangle& rectangle, const Vector2& offset={0,0}, float rotation=0.0f) { this->rectangle = rectangle; this->offset = offset; this->rotation = rotation; isCircle = false; }
    void setRectangle(float x, float y, float width, float height, const Vector2& offset={0,0}, float rotation=0.0f) { rectangle = Rectangle{x,y,width,height}; this->offset = offset; this->rotation = rotation; isCircle = false; }

    Circle getCircle() const { return (isCircle) ? Circle{circle.center+offset,circle.radius} : Circle{{0,0},0}; }
    Rect getRectangle() const { return (!isCircle) ? Rect{rectangle.x+offset.x,rectangle.y+offset.y,rectangle.width,rectangle.height, rotation}: Rect{0,0,0,0}; }

    operator Circle() const { return (isCircle) ? Circle{circle.center+offset,circle.radius} : Circle{{0,0},0}; }
    operator Rect() const { return (!isCircle) ? Rect{rectangle.x+offset.x,rectangle.y+offset.y,rectangle.width,rectangle.height, rotation}: Rect{0,0,0,0}; }
};
