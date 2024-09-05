#pragma once

#include "../Component.hpp"
#include "../CollisionManager.hpp"
#include "Transform2D.hpp"

class Transform2D;
class CollisionShape;
class CollisionManager;
class Component;


class Collider: public Component
{
public:
    Collider();
    ~Collider();

    nlohmann::json serialize() override;

    void deserialize(const nlohmann::json &j) override;

    void update(float deltatime) override;
    void renderUpdate(float renderDeltaTime) override;
    void physicsUpdate(float fixedDeltaTime) override;

    std::shared_ptr<CollisionShape> addCollisionShape(const CollisionShape& shape);
    std::shared_ptr<CollisionShape> addCollisionShape(const Circle& circle);
    std::shared_ptr<CollisionShape> addCollisionShape(std::shared_ptr<CollisionShape> shape);
    std::shared_ptr<CollisionShape> addCollisionShape(const Rectangle& rectangle, float rotation=0);

    void removeCollisionShape(const std::shared_ptr<CollisionShape> &shape);

    std::vector<std::shared_ptr<CollisionShape>> getShapes() const { return shapes; }

    Rectangle drawInspector(Rectangle& rectangle) override;

private:
    mutable std::shared_ptr<Transform2D> transform = nullptr;
    std::vector<std::shared_ptr<CollisionShape>> shapes;

};

class CollisionShape {
public:

    GameObject* owner;
    Collider* collider;
    bool isCircle;
    Circle circle;
    Rectangle rectangle;
    float rotation;
    // std::vector<int> layer;
    // std::vector<int> mask;
    std::vector<int> editorEditMode;
    std::shared_ptr<Transform2D> transform = nullptr;

    nlohmann::json serialize() ;
    void deserialize(const nlohmann::json &j);


    void getTransform();
    Rectangle drawInspector(Rectangle& rectangle);

    CollisionShape(const Circle& circle);
    CollisionShape(const Rectangle& rectangle, float rotation);
    CollisionShape();

    void setGameObject(GameObject* owner);
    void setCollider(Collider* collider);
    [[nodiscard]] GameObject* getGameObject() const { return owner; }
    [[nodiscard]] Collider* getCollider() const { return collider; }

    Rectangle getBoundingBox();

    void setRotation(float rotation) { this->rotation = rotation; }
    void setCircle(const Circle& circle) { this->circle = Circle{circle.center,circle.radius}; isCircle = true; }
    void setCircle(const Vector2& center, float radius) { circle = Circle{center, radius}; isCircle = true; }

    void setRectangle(const Rectangle& rectangle, float rotation=0.0f) { this->rectangle = rectangle; this->rotation = rotation; isCircle = false; }
    void setRectangle(float x, float y, float width, float height, float rotation=0.0f) { rectangle = Rectangle{x,y,width,height}; this->rotation = rotation; isCircle = false; }

    Circle getCircle() const;
    Rect getRectangle() const;

    operator Circle() const { return (isCircle) ? Circle{circle.center,circle.radius} : Circle{{0,0},0}; }
    operator Rect() const { return (!isCircle) ? Rect{rectangle.x,rectangle.y,rectangle.width,rectangle.height, rotation}: Rect{0,0,0,0}; }

    //bool ifCircle() {return isCircle;}
    int whatShape(){ return (isCircle)? 1 : 2;}
};
