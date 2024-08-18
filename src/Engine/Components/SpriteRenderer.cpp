#include "SpriteRenderer.hpp"
#include "../Component.hpp"
#include "Transform2D.hpp"
#include "raygui.h"
#include <sys/stat.h>


nlohmann::json SpriteRenderer::serialize() { 
    nlohmann::json json;
    json["componentType"] = name;
    json["isActive"] = isActive;
    json["offset"] = {offset.x, offset.y};
    json["scale"] = scale;
    json["rotation"] = rotation;
    json["filename"] = filename;
    json["size"] = {size.x, size.y};
    return json;
}

void SpriteRenderer::deserialize(const nlohmann::json& json)
{
    isActive = json["isActive"].get<bool>();

    offset.x = json["offset"][0].get<float>();
    offset.y = json["offset"][1].get<float>();

    scale = json["scale"].get<float>();

    rotation = json["rotation"].get<float>();

    filename = json["filename"].get<std::string>();

    size.x = json["size"][0].get<float>();
    size.y = json["size"][1].get<float>();

    // // If the texture needs to be loaded from the file
    // loadImage(filename.c_str());
    // initTexture();
    loadImage(filename);
    resizeImage(size.x, size.y);
    initTexture();
}

SpriteRenderer::SpriteRenderer() : offset(Vector2()), size(Vector2()) {
    // getTransform();
    name = "SpriteRenderer";
    texture = {0};
    image = {};
};

SpriteRenderer::~SpriteRenderer() {
    if (texture.id) {
        UnloadTexture(texture);
    }
}

void SpriteRenderer::loadImage(const std::string &filename) {
    std::lock_guard<std::mutex> lock(spriteMutex);
    struct stat buffer{};
    if (stat(filename.c_str(), &buffer) != 0) {
        std::cerr << "File does not exist: " << filename << std::endl;
        return;
    }
    if (image.data) {
        UnloadImage(image);
    }
    this->filename = filename;
    image = LoadImage(filename.c_str());
    isTextureInitialized = false;
}

void SpriteRenderer::resizeImage(const int width, const int height, const bool useNearestNeighbour) {
    if (useNearestNeighbour) {
        ImageResizeNN(&image, width, height);
    } else {
        ImageResize(&image, width, height);
    }
    size = {static_cast<float>(image.width), static_cast<float>(image.height)};
    isTextureInitialized = false;
}

void SpriteRenderer::setImage(const Image &image) {
    std::lock_guard<std::mutex> lock(spriteMutex);
    if (this->image.data) {
        UnloadImage(this->image);
    }
    this->image = image;
    isTextureInitialized = false;
}

void SpriteRenderer::setTexture(const Texture2D &texture) {
    std::lock_guard<std::mutex> lock(spriteMutex);
    if (this->texture.id) {
        UnloadTexture(this->texture);
    }
    this->texture = texture;
    size = {static_cast<float>(texture.width), static_cast<float>(texture.height)};
    isTextureInitialized = true;
}

void SpriteRenderer::initTexture() {
    if (image.data) {
        if (texture.id) {
            UnloadTexture(texture);
        }
        texture = LoadTextureFromImage(image);
        size = {static_cast<float>(texture.width), static_cast<float>(texture.height)};
        isTextureInitialized = true;
        UnloadImage(image);
    } else {
        std::cerr << "Image not loaded" << std::endl;
    }
}

void SpriteRenderer::renderUpdate(float renderDeltaTime) {
    std::lock_guard<std::mutex> lock(spriteMutex);
    if (!isActive) {
        return;
    }
    if (!transform) {
        getTransform();
    }
    if(!isTextureInitialized && !image.data && !filename.empty()) {
        loadImage(filename);
        resizeImage(size.x, size.y);
    }
    if (!texture.id) {
        initTexture();
    }
    draw();
}

Rectangle SpriteRenderer::drawInspector(Rectangle &previousRectangle) {
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

    // Draw and update height for each UI element

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Offset");
    GuiTextBox(Rectangle{ textBoxX, currentY, textBoxWidth / 2.0f - 5.0f, 20 }, &std::to_string(offset.x)[0u], 10, true); // x
    GuiTextBox(Rectangle{ textBoxX + textBoxWidth / 2.0f + 5.0f, currentY, textBoxWidth / 2.0f - 5.0f, 20 }, &std::to_string(offset.y)[0u], 10, true); // y
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Scale");
    GuiTextBox(Rectangle{ textBoxX, currentY, textBoxWidth, 20 }, &std::to_string(scale)[0u], 10, true); // scale
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Rotation");
    GuiTextBox(Rectangle{ textBoxX, currentY, textBoxWidth, 20 }, &std::to_string(rotation)[0u], 10, true); // rotation
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    GuiLabel(Rectangle{ labelX, currentY, labelWidth, 20 }, "Filename");
    GuiTextBox(Rectangle{ textBoxX, currentY, textBoxWidth, 20 }, &filename[0u], filename.length(), false); // filename
    currentY += 30.0f; // Update currentY to next line
    groupBoxHeight += 30.0f; // Increment height

    // Final group box position and size
    Rectangle groupBoxRect = {
        previousRectangle.x + padding,
        previousRectangle.y + previousRectangle.height + padding,
        groupBoxWidth,
        groupBoxHeight + padding // Add some padding at the bottom
    };

    // Draw the group box with the "SpriteRenderer" label
    GuiGroupBox(groupBoxRect, "SpriteRenderer");

    Rectangle returnRect = {
        previousRectangle.x,
        previousRectangle.y,
        previousRectangle.width,
        groupBoxHeight + padding * 5
    };

    // Return the bounding rectangle for the entire group box
    return returnRect;
}

void SpriteRenderer::draw() {
    if (!isTextureInitialized) {
        initTexture();
    }
    if (transform) {
        const Vector2 position = transform->getGamePosition();
        const float worldRotation = transform->getWorldRotation();
        const float worldScale = transform->getWorldScale();

        DrawTextureEx(texture, position + offset - size * scale * worldScale / 2, worldRotation + rotation, scale, WHITE);
        // std::cout << "Drawing sprite at: " << position.x << ", " << position.y << std::endl;
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
