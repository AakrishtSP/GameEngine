// #include "Component.hpp"
// #include "Components/Components.hpp"
#include "Base.hpp"
#include <filesystem>

#include "rlgl.h"


int main() {
    // Initialization
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;

    const std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Working Directory: " << currentPath << std::endl;

    InitWindow(screenWidth, screenHeight, "Raylib Game Engine");

    while (!IsWindowReady());

    GameObject root("Root");
    auto rootTransform = root.addComponent<Transform2D>();
    auto &child1 = root.addChild("Child1");
    auto &child2 = root.addChild("Child2");

    std::shared_ptr<Transform2D> child1Transform = child1.addComponent<Transform2D>();
    std::shared_ptr<SpriteRenderer> child1Sprite = child1.addComponent<SpriteRenderer>();
    child1Sprite->loadImage("./assets/Raylib_logo.png");
    child1Sprite->resizeImage(100, 100);
    child1Sprite->initTexture();
    child1Transform->setPosition({0, 0});
    child1Sprite->getTransform();

    const auto child2Transform = child2.addComponent<Transform2D>();
    const auto child2Sprite = child2.addComponent<SpriteRenderer>();

    child2Sprite->loadImage("./assets/Raylib_logo.png");
    child2Sprite->resizeImage(150, 150);
    child2Sprite->initTexture();
    child2Transform->setPosition({-100, -100});
    child2Sprite->getTransform();

    SetTargetFPS(60); // Set the game to run at 60 frames-per-second

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Draw
        BeginDrawing();
        // Set up the custom coordinate system
        rlPushMatrix();
        // Translate the origin to the center of the window
        rlTranslatef(screenWidth / 2.0f, screenHeight / 2.0f, 0);

        // Invert the Y-axis (scale Y by -1)
        rlScalef(1.0f, -1.0f, 1.0f);

        ClearBackground(GRAY); // Clear the screen to white


        if (IsKeyDown(KEY_W)) {
            child1Transform->translate({0, 1});
        }
        if (IsKeyDown(KEY_S)) {
            child1Transform->translate({0, -1});
        }
        if (IsKeyDown(KEY_A)) {
            child1Transform->translate({-1, 0});
        }
        if (IsKeyDown(KEY_D)) {
            child1Transform->translate({1, 0});
        }

        if (IsKeyDown(KEY_UP)) {
            child2Transform->translate({0, 1});
        }
        if (IsKeyDown(KEY_DOWN)) {
            child2Transform->translate({0, -1});
        }
        if (IsKeyDown(KEY_LEFT)) {
            child2Transform->translate({-1, 0});
        }
        if (IsKeyDown(KEY_RIGHT)) {
            child2Transform->translate({1, 0});
        }

        // Draw axes using the custom coordinate system
        DrawLine(-screenWidth / 2, 0, screenWidth / 2, 0, RED); // X-axis
        DrawLine(0, -screenHeight / 2, 0, screenHeight / 2, GREEN); // Y-axis


        root.update();

        // Display the FPS
        DrawText( std::to_string(GetFPS()).c_str(), 10, 10, 20, BLACK);
        std::cout << "FPS: " << GetFPS() << std::endl;


        // Restore the original coordinate system
        DrawCircle(0, 0, 5,RED);
        rlPopMatrix();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
