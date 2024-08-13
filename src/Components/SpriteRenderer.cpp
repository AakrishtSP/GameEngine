#include "SpriteRenderer.hpp"
#include "../Component.hpp"
#include "Transform2D.hpp"
#include <sys/stat.h>


SpriteRenderer::SpriteRenderer(): offset(Vector2()), size(Vector2()) {
    // getTransform();
    texture = {0};
    image = {0};
};

SpriteRenderer::~SpriteRenderer() {
    UnloadTexture(texture);
}

void SpriteRenderer::loadImage(const std::string &filename) {
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) != 0) {
        std::cerr << "File does not exist: " << filename << std::endl;
        return;
    }
    if (image.data) {
        UnloadImage(image);
    }
    image = LoadImage(filename.c_str());
}

void SpriteRenderer::resizeImage(int width, int height, bool useNearestNeighbour) {
    if (useNearestNeighbour) {
        ImageResizeNN(&image, width, height);
    } else {
        ImageResize(&image, width, height);
    }
}

void SpriteRenderer::setImage(const Image &image) {
    if (this->image.data) {
        UnloadImage(this->image);
    }
    this->image = image;
}

void SpriteRenderer::setTexture(const Texture2D &texture) {
    if (this->texture.id) {
        UnloadTexture(this->texture);
    }
    this->texture = texture;
}

void SpriteRenderer::initTexture() {
    if (image.data) {
        if (texture.id) {
            UnloadTexture(texture);
        }
        texture = LoadTextureFromImage(image);
        size = {static_cast<float>(texture.width), static_cast<float>(texture.height)};
        UnloadImage(image);
    } else {
        std::cerr << "Image not loaded" << std::endl;
    }
}

void SpriteRenderer::update() {
    if (!isActive) {
        return;
    }
    if (!transform) {
        getTransform();
    }
    if (!texture.id) {
        initTexture();
    }
    draw();
}

void SpriteRenderer::draw() const {
    if (transform) {
        Vector2 position = transform->getGamePosition();
        float worldRotation = transform->getWorldRotation();
        float worldScale = transform->getWorldScale();

        DrawTextureEx(texture, position + offset - size * scale * worldScale / 2, worldRotation + rotation, scale,
                      {255, 255, 255, 255});
    } else {
        std::cerr << "Transform not available for drawing." << std::endl;
    }
}

void SpriteRenderer::getTransform() {
    if (!owner) {
        std::cerr << "owner not found" << std::endl;
        return;
    }
    transform = owner->getComponent<Transform2D>();
    if (!transform) {
        transform = owner->addComponent<Transform2D>();
    }
}
