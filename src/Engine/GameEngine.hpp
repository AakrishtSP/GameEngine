#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <thread>
#include "Base.hpp"

// #ifndef RAYGUI_IMPLEMENTATION
// #define RAYGUI_IMPLEMENTATION
// #endif

// #include "raygui.h"

class GameEngine {
public:
    static GameEngine& getInstance();

    void run();

private:
    GameEngine();
    ~GameEngine();

    void init();
    void logicLoop();
    void physicsLoop();
    void renderLoop();

    std::mutex renderMutex;
    std::mutex physicsMutex;

    std::thread physicsThread;
    std::thread logicThread;

    GameObject root;

    float physicsUpdateInterval;
    float renderUpdateInterval;
    float logicUpdateInterval;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastRenderTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastPhysicsTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastLogicThreadUpdateTime;

    bool showMessageBox;

    // Deleted to prevent copying
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
};

