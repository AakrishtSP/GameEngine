#include "GameEngine.hpp"

GameEngine &GameEngine::getInstance() {
    static GameEngine instance;
    return instance;
}

GameEngine::GameEngine() :
    targetPhysicsUpdateInterval(1.0f / 60.0f), targetRenderUpdateInterval(1.0f / 60.0f), targetLogicUpdateInterval(1.0f / 60.0f),
    worldGravity({0.0f, 9.8f}), isPlaying(false) {
    lastRenderThreadUpdateTime = std::chrono::high_resolution_clock::now();
    lastPhysicsThreadUpdateTime = std::chrono::high_resolution_clock::now();
    lastLogicThreadUpdateTime = std::chrono::high_resolution_clock::now();
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
    registerComponents();

    worldGravity = {0.0f, -9.8f};

    root = std::make_shared<GameObject>("Root");

    SetTraceLogLevel(LOG_WARNING);

    InitWindow(screenWidth, screenHeight, "Raylib Game Engine");

    while (!IsWindowReady())
        ;

    const std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Working Directory: " << currentPath << std::endl;

    Editor::getInstance().init();
    CollisionManager::getInstance();


    SetTargetFPS(60); // Set the target frames-per-second
}

void GameEngine::deserializeScene(const std::string &scenePath) {
    std::ifstream f(scenePath);
    nlohmann::json data = nlohmann::json::parse(f);
    f.close();
    root->deserialize(data);
}

void GameEngine::serializeScene(const std::string &scenePath) {
    nlohmann::json rootJson = root->serialize();
    std::ofstream o(scenePath);
    o << std::setw(4) << rootJson << std::endl;
    o.close();
}

void GameEngine::run() {
    // Start threads for physics and rendering
    physicsThread = std::thread(&GameEngine::physicsLoop, this);
    logicThread = std::thread(&GameEngine::logicLoop, this);

    renderLoop();

    // Wait for threads to finish

    serializeScene("../Data/Outdata.json");
}

void GameEngine::logicLoop() {
    while (!WindowShouldClose()) {
        if (isPlaying) {
            auto now = std::chrono::high_resolution_clock::now();
            auto logicThreadElapsed = std::chrono::duration<float>(now - lastLogicThreadUpdateTime).count();

            if (logicThreadElapsed >= targetLogicUpdateInterval) {
                root->update(logicThreadElapsed);

                lastLogicUpdateTime = logicThreadElapsed;
                lastLogicThreadUpdateTime = now;
            }

        } else {
            lastLogicThreadUpdateTime = std::chrono::high_resolution_clock::now();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to avoid busy-waiting
    }
}

void GameEngine::physicsLoop() {
    while (!WindowShouldClose()) {
        if (isPlaying) {
            auto now = std::chrono::high_resolution_clock::now();
            auto physicsElapsed = std::chrono::duration<float>(now - lastPhysicsThreadUpdateTime).count();

            if (physicsElapsed >= targetPhysicsUpdateInterval) {
                std::lock_guard<std::mutex> lock(physicsMutex);
                root->physicsUpdate(physicsElapsed);
                CollisionManager::getInstance().update(physicsElapsed);

                lastPhysicsUpdateTime = physicsElapsed;
                lastPhysicsThreadUpdateTime = now;
            }

        } else {
            lastPhysicsThreadUpdateTime = std::chrono::high_resolution_clock::now();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to avoid busy-waiting
    }
}

void GameEngine::renderLoop() {
    Editor &editor = Editor::getInstance();

    while (!WindowShouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration<float>(now - lastRenderThreadUpdateTime).count();

        if (elapsed >= targetRenderUpdateInterval) {
            BeginDrawing();
            ClearBackground(RAYWHITE); // Clear the screen to gray

            if (!isPlaying) {
                DrawLine(0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, RED); // X-axis
                DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), GREEN); // Y-axis
            }

            std::lock_guard<std::mutex> lock(renderMutex);
            root->renderUpdate(elapsed);

            editor.draw();

            lastRenderUpdateTime = elapsed;
            lastRenderThreadUpdateTime = now;
            EndDrawing();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to avoid busy-waiting
    }
}
