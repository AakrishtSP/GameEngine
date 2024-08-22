#include "Collider.hpp"

Collider::Collider() {
    name = "Collider";
}

Collider::~Collider() {}

nlohmann::json Collider::serialize() { 
    nlohmann::json json;
    json["componentType"] = name;
    json["shapes"] = nlohmann::json::array();
    for (auto &shape : shapes) {
        json["shapes"].push_back(shape->serialize());
    }
    return json;
}

void Collider::deserialize(const nlohmann::json &json) {
    for (auto &shape : json["shapes"]) {
        std::shared_ptr<CollisionShape> shapePtr = std::make_shared<CollisionShape>();
        shapePtr->deserialize(shape);
        shapes.push_back(shapePtr);
    }
}

void Collider::update(float deltaTime) {}

void Collider::physicsUpdate(float fixedDeltaTime) {}

std::shared_ptr<CollisionShape> Collider::addCollisionShape(const CollisionShape &shape) {
    std::shared_ptr<CollisionShape> shapePtr = std::make_shared<CollisionShape>(shape);
    shapes.push_back(shapePtr);
    return shapePtr;
}

std::shared_ptr<CollisionShape> Collider::addCollisionShape(const Circle &circle, const Vector2 &offset) {
    std::shared_ptr<CollisionShape> shapePtr = std::make_shared<CollisionShape>(circle, offset);
    shapes.push_back(shapePtr);
    return shapePtr;
}

std::shared_ptr<CollisionShape> Collider::addCollisionShape(const Rectangle &rectangle, const Vector2 &offset,
                                                            float rotation) {
    std::shared_ptr<CollisionShape> shapePtr = std::make_shared<CollisionShape>(rectangle, offset, rotation);
    shapes.push_back(shapePtr);
    return shapePtr;
}

void Collider::removeCollisionShape(const std::shared_ptr<CollisionShape> &shape) {
    shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
}

nlohmann::json CollisionShape::serialize() { 
    nlohmann::json json;
    json["isCircle"] = isCircle;
    json["offset"] = {offset.x, offset.y};
    json["rotation"] = rotation;
    json["circle"] = {circle.center.x, circle.center.y, circle.radius};
    json["rectangle"] = {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
    return json; 
}

void CollisionShape::deserialize(const nlohmann::json &json) {
    isCircle = json["isCircle"].get<bool>();
    offset.x = json["offset"][0].get<float>();
    offset.y = json["offset"][1].get<float>();
    rotation = json["rotation"].get<float>();

    circle.center.x = json["circle"][0].get<float>();
    circle.center.y = json["circle"][1].get<float>();
    circle.radius = json["circle"][2].get<float>();

    rectangle.x = json["rectangle"][0].get<float>();
    rectangle.y = json["rectangle"][1].get<float>();
    rectangle.width = json["rectangle"][2].get<float>();
    rectangle.height = json["rectangle"][3].get<float>();
}
