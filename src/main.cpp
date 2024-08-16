#include "Engine/Base.hpp"
#include <filesystem>
#include <fstream>

#include "rlgl.h"

int main() {
    // Initialization
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;
    SetTraceLogLevel(LOG_WARNING);
    registerComponents();

    const std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Working Directory: " << currentPath << std::endl;

    InitWindow(screenWidth, screenHeight, "Raylib Game Engine");

    while (!IsWindowReady());

    GameObject root("Root");
    auto rootTransform = root.addComponent<Transform2D>();


    std::ifstream f("../Data/data.json");
    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    root.deserialize(data);

    nlohmann::json rootJson = root.serialize();
    std::ofstream o("../Data/Outdata.json");
    o << std::setw(4) << rootJson << std::endl;
    o.close();

    SetTargetFPS(60); // Set the game to run at 60 frames-per-second

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Draw
        BeginDrawing();

        ClearBackground(GRAY); // Clear the screen to white

        // Draw axes using the custom coordinate system
        
        DrawLine(0, GetScreenHeight()/2, GetScreenWidth(), GetScreenHeight()/2, RED); // X-axis
        DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), GREEN); // Y-axis
        root.update();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
