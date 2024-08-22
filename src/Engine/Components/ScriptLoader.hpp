#pragma once

#include <dlfcn.h>
#include "../Component.hpp"
#include "../ScriptInterface.hpp"

class ScriptInterface;

class ScriptLoader : public Component {
public:
    ScriptLoader();
    ~ScriptLoader() override;

    nlohmann::json serialize() override;
    void deserialize(const nlohmann::json &j) override;

    // Override Component methods
    void update(float deltaTime) override;
    void physicsUpdate(float fixedDeltaTime) override;
    void renderUpdate(float renderDeltaTime) override;

    // Script management
    bool loadScript(const std::string& scriptPath);
    void unloadScript();
    void callFunction(const std::string& functionName);
    
    // Set context for the script
    void setGameObject(GameObject* gameObject);
private:
    void* scriptHandle = nullptr;
    ScriptInterface* scriptInstance = nullptr;

    std::string scriptPath;

    using CreateFuncType = ScriptInterface*(*)();
    using DestroyFuncType = void(*)(ScriptInterface*);

    float deltaTime = 0.0f;
    float fixedDeltaTime = 0.0f;
    float renderDeltaTime = 0.0f;

    CreateFuncType createFunc = nullptr;
    DestroyFuncType destroyFunc = nullptr;
};
