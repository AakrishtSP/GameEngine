#include "Collider.hpp"
#include "raygui.h"

Collider::Collider() {
    name = "Collider";
    editorEditMode.resize(6, 0);
}

Collider::~Collider() {}

nlohmann::json Collider::serialize() {
    nlohmann::json json;
    json["componentType"] = name;
    json["shapes"] = nlohmann::json::array();
    for (auto &shape: shapes) {
        json["shapes"].push_back(shape->serialize());
    }
    return json;
}

void Collider::deserialize(const nlohmann::json &json) {
    for (auto &shape: json["shapes"]) {
        std::shared_ptr<CollisionShape> shapePtr = std::make_shared<CollisionShape>();
        shapePtr->deserialize(shape);
        shapes.push_back(shapePtr);
    }
}

void Collider::update(float deltaTime) {}

void Collider::physicsUpdate(float fixedDeltaTime) {
    // CollisionManager::getInstance().addCollider(this);
}

std::shared_ptr<CollisionShape> Collider::addCollisionShape(const CollisionShape &shape) {
    std::shared_ptr<CollisionShape> shapePtr = std::make_shared<CollisionShape>(shape);
    shapes.push_back(shapePtr);
    return shapePtr;
}

std::shared_ptr<CollisionShape> Collider::addCollisionShape(const Circle &circle) {
    std::shared_ptr<CollisionShape> shapePtr = std::make_shared<CollisionShape>(circle);
    shapes.push_back(shapePtr);
    return shapePtr;
}

std::shared_ptr<CollisionShape> Collider::addCollisionShape(const Rectangle &rectangle,
                                                            float rotation) {
    std::shared_ptr<CollisionShape> shapePtr = std::make_shared<CollisionShape>(rectangle, rotation);
    shapes.push_back(shapePtr);
    return shapePtr;
}

void Collider::removeCollisionShape(const std::shared_ptr<CollisionShape> &shape) {
    shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
}

Rectangle Collider::drawInspector(Rectangle &previousRectangle) {
    if (shapes.empty())
        return previousRectangle;

    // Define the width of the group box and padding
    float padding = 10.0f;
    float groupBoxWidth = previousRectangle.width - 2 * padding;

    // Initialize the height to accommodate the title of the group box
    float groupBoxHeight = padding;

    Rectangle shapeRect = previousRectangle;

    for (auto &shape: shapes) {
        shapeRect= shape->drawInspector(shapeRect);
        groupBoxHeight += shapeRect.height;
    }

    // Final group box position and size
    Rectangle groupBoxRect = {
            previousRectangle.x + padding, previousRectangle.y + previousRectangle.height + padding, groupBoxWidth,
            groupBoxHeight // Add some padding at the bottom
    };

    // Draw the group box with the "Transform2D" label
    GuiGroupBox(groupBoxRect, "Collider");

    Rectangle returnRect = {previousRectangle.x, previousRectangle.y + previousRectangle.height,
                            previousRectangle.width, groupBoxHeight + padding*5};

    // Return the bounding rectangle for the entire group box
    return returnRect;
}


CollisionShape::CollisionShape(const Circle &circle): circle(circle), isCircle(true), rotation(0), rectangle({0, 0, 0, 0}) {
    editorEditMode.resize(6, 0);
    }
CollisionShape::CollisionShape(const Rectangle &rectangle, float rotation = 0) :
    rectangle(rectangle), rotation(rotation), isCircle(false) {
    editorEditMode.resize(6, 0);
    }
CollisionShape::CollisionShape() : isCircle(false), rectangle({0, 0, 0, 0}), rotation(0), circle({{0, 0}, 0}) {
    editorEditMode.resize(6, 0);
}
void CollisionShape::setGameObject(GameObject *owner) {
    this->owner = owner;
    collider = owner->getComponent<Collider>().get();
}
void CollisionShape::setCollider(Collider *collider) {
    this->collider = collider;
    owner = collider->getOwner();
}


Rectangle CollisionShape::getBoundingBox() const {
    std::shared_ptr<Transform2D> transform = owner->getComponent<Transform2D>();
    if (transform == nullptr) {
        return {0, 0, 0, 0};
    }
    Rectangle rect;
    rect.x = transform->getWorldPosition().x;
    rect.y = transform->getWorldPosition().y;
    if (isCircle) {
        rect.width = circle.radius * 2;
        rect.height = circle.radius * 2;
    } else {
        float rot = transform->getWorldRotation() + rotation;
        float sinRot = sinf(rot);
        float cosRot = cosf(rot);
        Vector2 halfSize = {rectangle.width / 2, rectangle.height / 2};
        Vector2 rotatedHalfSize = {halfSize.x * cosRot - halfSize.y * sinRot,
                                   halfSize.x * sinRot + halfSize.y * cosRot};
        rect.width = fabsf(rotatedHalfSize.x) * 2;
        rect.height = fabsf(rotatedHalfSize.y) * 2;
    }
    return rect;
}
Circle CollisionShape::getCircle() const { 
    Circle c;
    c.center = owner->getComponent<Transform2D>()->getWorldPosition()+circle.center;
    c.radius = circle.radius;
    return c;
 }
Rect CollisionShape::getRectangle() const { 
    Rect r;
    r.x = owner->getComponent<Transform2D>()->getWorldPosition().x+rectangle.x;
    r.y = owner->getComponent<Transform2D>()->getWorldPosition().y+rectangle.y;
    r.width = rectangle.width;
    r.height = rectangle.height;
    r.rotation = rotation;
    return r;
};

nlohmann::json CollisionShape::serialize() {
    nlohmann::json json;
    json["isCircle"] = isCircle;
    json["rotation"] = rotation;
    json["circle"] = {circle.center.x, circle.center.y, circle.radius};
    json["rectangle"] = {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
    return json;
}

void CollisionShape::deserialize(const nlohmann::json &json) {
    isCircle = json["isCircle"].get<bool>();
    rotation = json["rotation"].get<float>();

    circle.center.x = json["circle"][0].get<float>();
    circle.center.y = json["circle"][1].get<float>();
    circle.radius = json["circle"][2].get<float>();

    rectangle.x = json["rectangle"][0].get<float>();
    rectangle.y = json["rectangle"][1].get<float>();
    rectangle.width = json["rectangle"][2].get<float>();
    rectangle.height = json["rectangle"][3].get<float>();
}

Rectangle CollisionShape::drawInspector(Rectangle &previousRectangle) {
    // Define the width of the group box and padding
    float padding = 10.0f;
    float groupBoxWidth = previousRectangle.width - 2 * padding;

    // Initialize the height to accommodate the title of the group box
    float groupBoxHeight = padding;
    float currentY = previousRectangle.y + previousRectangle.height + padding;

    // Calculate positions for the first label and text boxes
    float labelX = previousRectangle.x + padding;
    float labelWidth = previousRectangle.width / 3.0f;
    float textBoxX = labelX + labelWidth + 10.0f;
    float textBoxWidth = groupBoxWidth - labelWidth - 3 * padding;

    GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Is Circle");
    GuiCheckBox(Rectangle{textBoxX, currentY, 20, 20}, "", &isCircle); // isMoveable
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    if (isCircle) {

        GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Center");
        editorEditMode.at(0) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                                          &std::to_string(circle.center.x)[0u], 10, editorEditMode[0]); // x
        editorEditMode.at(1) =
                GuiTextBox(Rectangle{textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                           &std::to_string(circle.center.y)[0u], 10, editorEditMode[1]); // y
        currentY += 30.0f; // Update currentY to next line
        groupBoxHeight += 30.0f; // Increment height

        GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Radius");
        editorEditMode.at(2) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth, 20},
                                          &std::to_string(circle.radius)[0u], 10, editorEditMode[2]); // radius
        currentY += 30.0f; // Update currentY to next line
        groupBoxHeight += 30.0f; // Increment height

    } else {

        GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Position");
        editorEditMode.at(0) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                                          &std::to_string(rectangle.x)[0u], 10, editorEditMode[0]); // x
        editorEditMode.at(1) =
                GuiTextBox(Rectangle{textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                           &std::to_string(rectangle.y)[0u], 10, editorEditMode[1]); // y
        currentY += 30.0f; // Update currentY to next line
        groupBoxHeight += 30.0f; // Increment height

        GuiLabel(Rectangle{labelX, currentY, labelWidth, 20}, "Size");
        editorEditMode.at(2) = GuiTextBox(Rectangle{textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                                          &std::to_string(rectangle.width)[0u], 10, editorEditMode[2]); // width
        editorEditMode.at(3) =
                GuiTextBox(Rectangle{textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20},
                           &std::to_string(rectangle.height)[0u], 10, editorEditMode[3]); // height
        currentY += 30.0f; // Update currentY to next line
        groupBoxHeight += 30.0f; // Increment height
    }

    Rectangle returnRect = {previousRectangle.x, previousRectangle.y + previousRectangle.height,
                            previousRectangle.width, groupBoxHeight + padding};

    // Return the bounding rectangle for the entire group box
    return returnRect;
}
