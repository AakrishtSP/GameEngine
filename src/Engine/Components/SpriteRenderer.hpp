#pragma once
#include "../Component.hpp"
#include "Transform2D.hpp"
#include <sys/stat.h>


class Transform2D;
class GameObject;


class SpriteRenderer : public Component
{
public:
    SpriteRenderer(); 
    ~SpriteRenderer() override;

    void loadImage(const std::string& filename);
    void resizeImage(int width, int height, bool useNearestNeighbour = false);
    void setImage(const Image& image);
    void setTexture(const Texture2D& texture);
    Image* getImage() { return &image; }
    Texture2D getTexture() const { return texture; }
    void initTexture();

    void setOffset(const Vector2& offset) { this->offset = offset; }
    void setScale(const float scale) { this->scale = scale; }
    void setRotation(const float rotation) { this->rotation = rotation; }

    void update() override;
    void draw() const;
    void getTransform() const;
    

protected:
    mutable std::shared_ptr<Transform2D> transform = nullptr;
    float scale = 1.0f;
    Vector2 offset{};
    Texture2D texture{};
    float rotation = 0.0f;
    Image image{};
    Vector2 size{};
};
