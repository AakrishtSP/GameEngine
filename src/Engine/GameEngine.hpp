#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <thread>
#include "Components/Components.hpp"
#include "Base.hpp"
#include "Editor.hpp"


class GameEngine {
public:
    static GameEngine& getInstance();

    std::shared_ptr<GameObject> getRoot(){return root;};

    void init();
    void run();

    friend class Editor;

private:
    GameEngine();
    ~GameEngine();

    void logicLoop();
    void physicsLoop();
    void renderLoop();

    void deserializeScene(const std::string& scenePath);
    void serializeScene(const std::string& scenePath);

    std::mutex renderMutex;
    std::mutex physicsMutex;

    std::thread physicsThread;
    std::thread logicThread;

    std::shared_ptr<GameObject> root;

    float physicsUpdateInterval;
    float renderUpdateInterval;
    float logicUpdateInterval;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastRenderTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastPhysicsTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastLogicThreadUpdateTime;

    bool showMessageBox;

    bool isPlaying;

    // Deleted to prevent copying
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
};

