#include "ScriptLoader.hpp"
#include <iostream>

ScriptLoader::ScriptLoader() {
    name = "ScriptLoader";
}

ScriptLoader::~ScriptLoader() {
    unloadScript();
}

nlohmann::json ScriptLoader::serialize() { 
    nlohmann::json json;
    json["componentType"] = name;
    json["isActive"] = isActive;
    json["scriptPath"] = scriptPath;
    return json;
}

void ScriptLoader::deserialize(const nlohmann::json &json) {
    isActive = json["isActive"].get<bool>();
    scriptPath = json["scriptPath"].get<std::string>();
    loadScript(scriptPath);
}

bool ScriptLoader::loadScript(const std::string& scriptPath) {
    unloadScript();  // Unload any previously loaded script

    scriptHandle = dlopen(scriptPath.c_str(), RTLD_LAZY);
    if (!scriptHandle) {
        std::cerr << "Failed to load script: " << dlerror() << std::endl;
        return false;
    }

    // Load the create and destroy functions
    createFunc = reinterpret_cast<CreateFuncType>(dlsym(scriptHandle, "createScript"));
    destroyFunc = reinterpret_cast<DestroyFuncType>(dlsym(scriptHandle, "destroyScript"));

    const char* dlsymError = dlerror();
    if (dlsymError) {
        std::cerr << "Failed to load functions: " << dlsymError << std::endl;
        unloadScript();
        return false;
    }

    // Create the script instance
    scriptInstance = createFunc();
    if (!scriptInstance) {
        std::cerr << "Failed to create script instance." << std::endl;
        unloadScript();
        return false;
    }

    // Set context for the script
    if (getOwner()) {
        scriptInstance->setOwner(getOwner());
    }
    this->scriptPath = scriptPath;
    scriptInstance->onStart();
    return true;
}

void ScriptLoader::unloadScript() {
    if (scriptInstance) {
        if (destroyFunc) {
            destroyFunc(scriptInstance);
        }
        scriptInstance = nullptr;
    }
    if (scriptHandle) {
        dlclose(scriptHandle);
        scriptHandle = nullptr;
    }
}

void ScriptLoader::callFunction(const std::string& functionName) {
    if (!scriptInstance) {
        std::cerr << "No script loaded." << std::endl;
        return;
    }

    // Call the function on the script instance
    if (functionName == "onStart") {
        scriptInstance->onStart();
    } else if (functionName == "onUpdate") {
        scriptInstance->onUpdate(deltaTime); // Provide a default deltaTime
    } else if (functionName == "onPhysicsUpdate") {
        scriptInstance->onPhysicsUpdate(fixedDeltaTime); // Provide a default fixedDeltaTime
    } else {
        std::cerr << "Function not found: " << functionName << std::endl;
    }
}

void ScriptLoader::update(float deltaTime) {
    this->deltaTime = deltaTime;
    callFunction("onUpdate");
}

void ScriptLoader::physicsUpdate(float fixedDeltaTime) {
    this->fixedDeltaTime = fixedDeltaTime;
    callFunction("onPhysicsUpdate");
}

void ScriptLoader::renderUpdate(float renderDeltaTime) {
    this->renderDeltaTime = renderDeltaTime;
    // Optional: Call render-related functions if defined
}

void ScriptLoader::setGameObject(GameObject* gameObject) {
    Component::setOwner(gameObject);
    if (scriptInstance) {
        scriptInstance->setOwner(gameObject);
    }
}
