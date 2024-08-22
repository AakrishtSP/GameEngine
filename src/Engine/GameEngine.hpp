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


    void init();
    void run();

    std::shared_ptr<GameObject> getRoot(){return root;};

    [[nodiscard]] float getPhysicsUpdateInterval() const { return lastPhysicsUpdateTime; }
    [[nodiscard]] float getRenderUpdateInterval() const { return lastRenderUpdateTime; }
    [[nodiscard]] float getLogicUpdateInterval() const { return lastLogicUpdateTime; }

    void setPhysicsUpdateInterval(float interval) { targetPhysicsUpdateInterval = interval; }
    void setRenderUpdateInterval(float interval) { targetRenderUpdateInterval = interval; }
    void setLogicUpdateInterval(float interval) { targetLogicUpdateInterval = interval; }

    void setWorldGravity(const Vector2 &gravity) { worldGravity = gravity; }
    [[nodiscard]] Vector2 getWorldGravity() const { return worldGravity; }

    
    void deserializeScene(const std::string& scenePath);
    void serializeScene(const std::string& scenePath);

    friend class Editor;

private:
    GameEngine();
    ~GameEngine();

    Vector2 worldGravity;

    void logicLoop();
    void physicsLoop();
    void renderLoop();


    std::mutex renderMutex;
    std::mutex physicsMutex;

    std::thread physicsThread;
    std::thread logicThread;

    std::shared_ptr<GameObject> root;

    float targetPhysicsUpdateInterval;
    float targetLogicUpdateInterval;
    float targetRenderUpdateInterval;

    float lastPhysicsUpdateTime;
    float lastLogicUpdateTime;
    float lastRenderUpdateTime;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastRenderThreadUpdateTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastPhysicsThreadUpdateTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastLogicThreadUpdateTime;

    bool isPlaying;

    // Deleted to prevent copying
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
};

