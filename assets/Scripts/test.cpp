#include "../../src/Engine/ScriptInterface.hpp"
#include "../../src/Engine/Base.hpp"
#include <iostream>

class Transform2D;
class ExampleScript : public ScriptInterface {
public:
    void setOwner(GameObject* owner) override {
        this->owner = owner;
    }

    void onStart() override {
        std::cout << "Script started!" << std::endl;
        std::cout << "Owner name: " << owner->getName() << std::endl;
        transform = owner->getComponent<Transform2D>();
    }

    void onUpdate(float deltaTime) override {


        // std::cout << "Script update: " << deltaTime << std::endl;
    }
    void onPhysicsUpdate(float fixedDeltaTime) override {
        if (IsKeyDown(KEY_W)) {
            transform->translate({0, 1});
        }
        if (IsKeyDown(KEY_S)) {
            transform->translate({0, -1});
        }
        if (IsKeyDown(KEY_A)) {
            transform->translate({-1, 0});
        }
        if (IsKeyDown(KEY_D)) {
            transform->translate({1, 0});
        }
        // std::cout << "Script physics update: " << fixedDeltaTime << std::endl;
    }

private:
    std::shared_ptr<Transform2D> transform = nullptr;
    GameObject* owner = nullptr;
};

extern "C" ScriptInterface* createScript() {
    return new ExampleScript();
}

extern "C" void destroyScript(ScriptInterface* script) {
    delete script;
}
