#include <chrono>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <thread>
#include "Engine/Base.hpp"

#include "rlgl.h"

std::mutex renderMutex; // Mutex for synchronizing access to rendering
std::mutex physicsMutex; // Mutex for synchronizing access to physics updates

void updatePhysics(GameObject &root, float fixedDeltaTime) {
    std::lock_guard<std::mutex> lock(physicsMutex);
    root.physicsUpdate(fixedDeltaTime);
    std::cout << "Physics update: " << fixedDeltaTime << std::endl;
}

void updateRender(GameObject &root, float deltaTime) {
    std::lock_guard<std::mutex> lock(renderMutex);
    root.renderUpdate(deltaTime);
    std::cout << "Render update: " << deltaTime << std::endl;
}

int main() {
    using namespace std::chrono;
    // Initialization
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;
    SetTraceLogLevel(LOG_WARNING);
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


    std::ifstream f("../Data/data.json");
    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    root.deserialize(data);

    nlohmann::json rootJson = root.serialize();
    std::ofstream o("../Data/Outdata.json");
    o << std::setw(4) << rootJson << std::endl;
    o.close();

    SetTargetFPS(60); // Set the game to run at 60 frames-per-second

    // Physics update thread
    std::thread physicsThread([&]() {
        while (!WindowShouldClose()) {
            auto now = high_resolution_clock::now();
            auto physicsElapsed = duration<float>(now - lastPhysicsTime).count();
            accumulatedPhysicsTime += physicsElapsed;
            lastPhysicsTime = now;

            while (accumulatedPhysicsTime >= physicsUpdateInterval) {
                updatePhysics(root, physicsUpdateInterval);
                accumulatedPhysicsTime -= physicsUpdateInterval;
            }

            std::this_thread::sleep_for(milliseconds(1)); // Sleep to avoid busy-waiting
        }
    });

    // Main thread update loop
    while (!WindowShouldClose()) {
        auto now = high_resolution_clock::now();
        auto mainThreadElapsed = duration<float>(now - lastMainThreadUpdateTime).count();

        if (mainThreadElapsed >= mainUpdateInterval) {
            root.update(mainUpdateInterval);
            std::cout << "Main update: " << mainUpdateInterval << std::endl;
            lastMainThreadUpdateTime = now;
        }

        std::this_thread::sleep_for(milliseconds(1)); // Sleep to avoid busy-waiting
    }
    // Rendering thread
    std::thread renderThread([&]() {
        while (!WindowShouldClose()) {
            auto now = high_resolution_clock::now();
            auto elapsed = duration<float>(now - lastRenderTime).count();

            if (elapsed >= renderUpdateInterval) {
                BeginDrawing();
                ClearBackground(GRAY); // Clear the screen to gray

                // Update render components
                updateRender(root, renderUpdateInterval);

                EndDrawing();
                lastRenderTime = now;
            }

            std::this_thread::sleep_for(milliseconds(1)); // Sleep to avoid busy-waiting
        }
    });

    while (!WindowShouldClose())
        ;

    


    // Join threads
    if (physicsThread.joinable()) {
        physicsThread.join();
    }

    if (renderThread.joinable()) {
        renderThread.join();
    }

    // while (!WindowShouldClose()) // Detect window close button or ESC key
    // {
    //     root.update(GetFrameTime());

    //     auto now = high_resolution_clock::now();
    //     auto elapsed = duration<float>(now - lastRenderTime).count();

    //     // Update physics at 120 FPS
    //     auto physicsElapsed = duration<float>(now - lastPhysicsTime).count();
    //     accumulatedPhysicsTime += physicsElapsed;
    //     lastPhysicsTime = now;

    //     while (accumulatedPhysicsTime >= physicsUpdateInterval) {
    //         root.physicsUpdate(physicsUpdateInterval);
    //         accumulatedPhysicsTime -= physicsUpdateInterval;
    //     }

    //     // Render at 60 FPS
    //     if (elapsed >= renderUpdateInterval) {
    //         BeginDrawing();
    //         ClearBackground(GRAY); // Clear the screen to gray
    //         DrawLine(0, GetScreenHeight()/2, GetScreenWidth(), GetScreenHeight()/2, RED); // X-axis
    //         DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), GREEN); // Y-axis

    //         // Update render components
    //         root.renderUpdate(elapsed);

    //         EndDrawing();
    //         lastRenderTime = now;
    //     }

    //     EndDrawing();
    // }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
