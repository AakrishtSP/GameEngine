#include "GameEngine.hpp"

GameEngine &GameEngine::getInstance() {
    static GameEngine instance;
    return instance;
}

GameEngine::GameEngine() :
    physicsUpdateInterval(1.0f / 120.0f), renderUpdateInterval(1.0f / 60.0f), logicUpdateInterval(1.0f / 60.0f),
    showMessageBox(false), root("Root") {
    lastRenderTime = std::chrono::high_resolution_clock::now();
    lastPhysicsTime = std::chrono::high_resolution_clock::now();
    lastLogicThreadUpdateTime = std::chrono::high_resolution_clock::now();

    registerComponents();
}

GameEngine::~GameEngine() {
    if (physicsThread.joinable()) {
        physicsThread.join();
    }
    if (logicThread.joinable()) {
        logicThread.join();
    }
    CloseWindow();
}

void GameEngine::init() {
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Raylib Game Engine");

    while (!IsWindowReady())
        ;

    const std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Working Directory: " << currentPath << std::endl;

    std::ifstream f("../Data/data.json");
    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    root.deserialize(data);

    nlohmann::json rootJson = root.serialize();
    std::ofstream o("../Data/Outdata.json");
    o << std::setw(4) << rootJson << std::endl;
    o.close();
}

void GameEngine::run() {
    init();

    // Start threads for physics and rendering
    physicsThread = std::thread(&GameEngine::physicsLoop, this);
    logicThread = std::thread(&GameEngine::logicLoop, this);

    renderLoop();

    // Join threads
    if (physicsThread.joinable()) {
        physicsThread.join();
    }
    if (logicThread.joinable()) {
        logicThread.join();
    }
}

void GameEngine::logicLoop() {
    while (!WindowShouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto logicThreadElapsed = std::chrono::duration<float>(now - lastLogicThreadUpdateTime).count();

        if (logicThreadElapsed >= logicUpdateInterval) {
            root.update(logicUpdateInterval);
            lastLogicThreadUpdateTime = now;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to avoid busy-waiting
    }
}

void GameEngine::physicsLoop() {
    float accumulatedPhysicsTime = 0.0f;

    while (!WindowShouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto physicsElapsed = std::chrono::duration<float>(now - lastPhysicsTime).count();
        accumulatedPhysicsTime += physicsElapsed;
        lastPhysicsTime = now;

        while (accumulatedPhysicsTime >= physicsUpdateInterval) {
            std::lock_guard<std::mutex> lock(physicsMutex);
            root.physicsUpdate(physicsUpdateInterval);
            accumulatedPhysicsTime -= physicsUpdateInterval;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to avoid busy-waiting
    }
}

void GameEngine::renderLoop() {
    while (!WindowShouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration<float>(now - lastRenderTime).count();

        BeginDrawing();
        if (elapsed >= renderUpdateInterval) {
            ClearBackground(GRAY); // Clear the screen to gray

            DrawLine(0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, RED); // X-axis
            DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), GREEN); // Y-axis

            // if (!showMessageBox) {
            //     int result = GuiMessageBox((Rectangle){85, 70, 250, 100}, "#191#Message Box", "Hi! This is a message!",
            //                                "Nice;Cool");
            // }

            std::lock_guard<std::mutex> lock(renderMutex);
            root.renderUpdate(renderUpdateInterval);

            lastRenderTime = now;
        }
        EndDrawing();
    }
}
