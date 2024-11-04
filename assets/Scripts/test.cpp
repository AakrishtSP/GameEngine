#include "../../src/Engine/ScriptInterface.hpp"
#include "../../src/Engine/Base.hpp"
#include <iostream>

// class Transform2D;
class ExampleScript : public ScriptInterface {
public:
    void setOwner(GameObject* owner) override {
        this->owner = owner;
    }

    void onStart() override {
        // std::cout << "Script started!" << std::endl;
        // std::cout << "Owner name: " << owner->getName() << std::endl;
        // transform = owner->getComponent<Transform2D>();
        // rigidBody = owner->getComponent<RigidBody2D>();
    }

    void onUpdate(float deltaTime) override {


        // std::cout << "Script update: " << deltaTime << std::endl;
    }
    void onPhysicsUpdate(float fixedDeltaTime) override {
        if (!rigidBody) {
            rigidBody = owner->getComponent<RigidBody2D>();
        }
        if (!transform) {
            transform = owner->getComponent<Transform2D>();
        }
        float multiplier = 20.0f;
        if (IsKeyDown(KEY_W)) {
            rigidBody->setVelocityY(multiplier);
            // rigidBody->addForce({0, multiplier});
        }
        if (IsKeyDown(KEY_S)) {
            rigidBody->setVelocityY( -multiplier);
            // rigidBody->addForce({0, -multiplier});
        }
        if (IsKeyDown(KEY_A)) {
            rigidBody->setVelocityX(-multiplier);
            // rigidBody->addForce({-multiplier, 0});
        }
        if (IsKeyDown(KEY_D)) {
            rigidBody->setVelocityX(multiplier);
            // rigidBody->addForce({multiplier, 0});
        }
        // std::cout << "Script physics update: " << fixedDeltaTime << std::endl;
    }

private:
    std::shared_ptr<Transform2D> transform = nullptr;
    std::shared_ptr<RigidBody2D> rigidBody = nullptr;
    GameObject* owner = nullptr;
};

extern "C" ScriptInterface* createScript() {
    return new ExampleScript();
}

extern "C" void destroyScript(ScriptInterface* script) {
    delete script;
}
