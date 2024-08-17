#pragma once
#include "../Component.hpp"



class Transform2D final : public Component
{
public:
    Transform2D() : position({0, 0}), rotation(0), scale(1), worldScale(0), worldPosition(), worldRotation(0) {
    }

    nlohmann::json serialize() override;
    void deserialize(const nlohmann::json &json) override;

    [[nodiscard]] Vector2 getPosition() const { return position; }
    void setPosition(const Vector2 &position) { this->position = position; }
    [[nodiscard]] float getRotation() const { return rotation; }
    void setRotation(const float rotation) { this->rotation = rotation; }
    [[nodiscard]] float getScale() const { return scale; }
    void setScale(const float &scale) { this->scale = scale; }
    [[nodiscard]] Vector2 getWorldPosition() const { return worldPosition; }
    [[nodiscard]] float getWorldRotation() const { return worldRotation; }
    [[nodiscard]] float getWorldScale() const { return worldScale; }
    void translate(const Vector2 &translation) { position = position + translation; }
    void rotate(const float rotation) { this->rotation += rotation; }

    void calculateWorldPosition();

    [[nodiscard]] Vector2 getGamePosition() const;

    void update(float deltaTime) override;

protected:
    Vector2 position;
    float rotation;
    float scale;
    float worldScale;
    Vector2 worldPosition;
    float worldRotation;
};
