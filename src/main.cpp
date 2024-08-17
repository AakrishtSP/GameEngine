#include <chrono>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <thread>
#include "Engine/Base.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// #include "rlgl.h"

std::mutex renderMutex; // Mutex for synchronizing access to rendering
std::mutex physicsMutex; // Mutex for synchronizing access to physics updates


void updateRender(GameObject *root, float deltaTime) {}

int main() {
    using namespace std::chrono;
    // Initialization
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;
    // SetTraceLogLevel(LOG_WARNING);

    registerComponents();

    const std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Working Directory: " << currentPath << std::endl;

    InitWindow(screenWidth, screenHeight, "Raylib Game Engine");

    while (!IsWindowReady())
        ;

    GameObject root("Root");

    auto lastRenderTime = high_resolution_clock::now();
    auto lastPhysicsTime = high_resolution_clock::now();
    auto lastMainThreadUpdateTime = high_resolution_clock::now();

    float physicsUpdateInterval = 1.0f / 120.0f; // 120 FPS
    float renderUpdateInterval = 1.0f / 60.0f; // 60 FPS
    float mainUpdateInterval = 1.0f / 60.0f; // 60 FPS


    float accumulatedPhysicsTime = 0.0f;

    bool showMessageBox = false;

    std::ifstream f("../Data/data.json");
    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    root.deserialize(data);

    nlohmann::json rootJson = root.serialize();
    std::ofstream o("../Data/Outdata.json");
    o << std::setw(4) << rootJson << std::endl;
    o.close();

    // SetTargetFPS(60); // Set the game to run at 60 frames-per-second

    // Physics update thread
    std::thread physicsThread([&]() {
        while (!WindowShouldClose()) {
            auto now = high_resolution_clock::now();
            auto physicsElapsed = duration<float>(now - lastPhysicsTime).count();
            accumulatedPhysicsTime += physicsElapsed;
            lastPhysicsTime = now;

            while (accumulatedPhysicsTime >= physicsUpdateInterval) {
                std::lock_guard<std::mutex> lock(physicsMutex);
                root.physicsUpdate(physicsUpdateInterval);
                // std::cout << "Physics update: " << physicsUpdateInterval << std::endl;
                accumulatedPhysicsTime -= physicsUpdateInterval;
            }

            std::this_thread::sleep_for(milliseconds(1)); // Sleep to avoid busy-waiting
        }
    });

    // Rendering thread
    std::thread renderThread([&]() {
        while (!WindowShouldClose()) {
            auto now = high_resolution_clock::now();
            auto mainThreadElapsed = duration<float>(now - lastMainThreadUpdateTime).count();

            if (mainThreadElapsed >= mainUpdateInterval) {
                root.update(mainUpdateInterval);
                // std::cout << "Main update: " << mainUpdateInterval << std::endl;
                lastMainThreadUpdateTime = now;
            }

            std::this_thread::sleep_for(milliseconds(1)); // Sleep to avoid busy-waiting
        }
    });


    // Render thread update loop
    while (!WindowShouldClose()) {
        auto now = high_resolution_clock::now();
        auto elapsed = duration<float>(now - lastRenderTime).count();

        BeginDrawing();
        if (elapsed >= renderUpdateInterval) {
            ClearBackground(GRAY); // Clear the screen to gray

            DrawLine(0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, RED); // X-axis
            DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), GREEN); // Y-axis


            if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Show Message")) showMessageBox = true;

            if (showMessageBox)
            {
                int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                    "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

                if (result >= 0) showMessageBox = false;
            }



            // Update render components
            std::lock_guard<std::mutex> lock(renderMutex);
            root.renderUpdate(renderUpdateInterval);
            // std::cout << "Render update: " << renderUpdateInterval << std::endl;

            lastRenderTime = now;
        }
        EndDrawing();

        // std::this_thread::sleep_for(milliseconds(1)); // Sleep to avoid busy-waiting
    }

    while (!WindowShouldClose())
        ;


    // Join threads
    if (physicsThread.joinable()) {
        physicsThread.join();
    }

    if (renderThread.joinable()) {
        renderThread.join();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
