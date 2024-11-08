#include "Engine/GameEngine.hpp"

int main() {
    GameEngine& engine = GameEngine::getInstance();
    engine.init();
    engine.setWorldGravity({0, -1000});
    engine.setPhysicsUpdateInterval(1.0f / 120.0f);

    // engine.deserializeScene("../Data/data.json");   //This can be used to load scene from json file

    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();
    float half_screen_width = screen_width / 2;
    float half_screen_height = screen_height / 2;

    std::shared_ptr<GameObject> root = engine.getRoot();

    auto rootTransform = root->addComponent<Transform2D>();
    auto rootCollider = root->addComponent<Collider>();

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
    auto rigidBody = child1.addComponent<RigidBody2D>();
    rigidBody->setMoveable(false);
    auto collider = child1.addComponent<Collider>();
    collider->addCollisionShape(Rectangle{0, 0, 50, 50});

    auto rb = child2.addComponent<RigidBody2D>();
    rb->setMoveable(false);
    const auto child2Transform = child2.addComponent<Transform2D>();
    const auto child2Sprite = child2.addComponent<SpriteRenderer>();
    auto collider2 = child2.addComponent<Collider>();

    script->loadScript("../assets/Build/libtest.so");
    child2Sprite->loadImage("../assets/cpp.png");
    child2Sprite->resizeImage(150, 150);
    child2Sprite->initTexture();
    child2Transform->setPosition({-50, -50});
    child2Sprite->getTransform();
    collider2->addCollisionShape(Rectangle{0, 0, 75, 75});

    //run engine
    engine.run();

    return 0;
}

