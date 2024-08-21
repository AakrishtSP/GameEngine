#include "GameEngine.hpp"

GameEngine &GameEngine::getInstance() {
    static GameEngine instance;
    return instance;
}

GameEngine::GameEngine() :
    physicsUpdateInterval(1.0f / 60.0f), renderUpdateInterval(1.0f / 60.0f), logicUpdateInterval(1.0f / 60.0f),
    showMessageBox(false) {
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

    root = std::make_shared<GameObject>("Root");

    SetTraceLogLevel(LOG_WARNING);

    InitWindow(screenWidth, screenHeight, "Raylib Game Engine");

    while (!IsWindowReady())
        ;

    const std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Working Directory: " << currentPath << std::endl;


    // deserializeScene("../Data/data.json");

    auto rootTransform = root->addComponent<Transform2D>();
    auto &child1 = root->addChild("Child1");
    auto &child2 = root->addChild("Child2");

    std::shared_ptr<Transform2D> child1Transform = child1.addComponent<Transform2D>();
    std::shared_ptr<SpriteRenderer> child1Sprite = child1.addComponent<SpriteRenderer>();
    child1Sprite->loadImage("../assets/Raylib_logo.png");
    child1Sprite->resizeImage(100, 100);
    child1Sprite->initTexture();
    child1Transform->setPosition({0, 0});
    child1Sprite->getTransform();
    auto script = child1.addComponent<ScriptLoader>();

    const auto child2Transform = child2.addComponent<Transform2D>();
    const auto child2Sprite = child2.addComponent<SpriteRenderer>();

    script->loadScript("../assets/Build/libtest.so");
    child2Sprite->loadImage("../assets/Raylib_logo.png");
    child2Sprite->resizeImage(150, 150);
    child2Sprite->initTexture();
    child2Transform->setPosition({-100, -100});
    child2Sprite->getTransform();

    Editor::getInstance().init();


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

            if (logicThreadElapsed >= logicUpdateInterval) {
                root->update(logicUpdateInterval);
                lastLogicThreadUpdateTime = now;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to avoid busy-waiting
        } else {
            lastLogicThreadUpdateTime = std::chrono::high_resolution_clock::now();
        }
    }
}

void GameEngine::physicsLoop() {
    float accumulatedPhysicsTime = 0.0f;

    while (!WindowShouldClose()) {
        if (isPlaying) {
            auto now = std::chrono::high_resolution_clock::now();
            auto physicsElapsed = std::chrono::duration<float>(now - lastPhysicsTime).count();
            accumulatedPhysicsTime += physicsElapsed;
            lastPhysicsTime = now;

            while (accumulatedPhysicsTime >= physicsUpdateInterval) {
                std::lock_guard<std::mutex> lock(physicsMutex);
                root->physicsUpdate(physicsUpdateInterval);
                accumulatedPhysicsTime -= physicsUpdateInterval;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to avoid busy-waiting
        } else {
            accumulatedPhysicsTime = 0.0f;
        }
    }
}

void GameEngine::renderLoop() {
    Editor &editor = Editor::getInstance();

    while (!WindowShouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration<float>(now - lastRenderTime).count();

        BeginDrawing();
        if (elapsed >= renderUpdateInterval) {
            ClearBackground(RAYWHITE); // Clear the screen to gray

            if (!isPlaying) {
                DrawLine(0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, RED); // X-axis
                DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), GREEN); // Y-axis
            }

            std::lock_guard<std::mutex> lock(renderMutex);
            root->renderUpdate(renderUpdateInterval);

            editor.draw();

            lastRenderTime = now;
        }
        EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to avoid busy-waiting
    }
}