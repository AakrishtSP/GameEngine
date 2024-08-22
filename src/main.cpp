#include "Engine/GameEngine.hpp"


int main() {
    GameEngine& engine = GameEngine::getInstance();
    engine.init();

    // engine.deserializeScene("../Data/data.json");

    std::shared_ptr<GameObject> root = engine.getRoot();

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
    auto rigidBody = child1.addComponent<RigidBody2D>();
    // rigidBody->setAffectedByGravity(1);
    // rigidBody->setMass(1);
    // rigidBody->setMoveable(true);

    child2.addComponent<RigidBody2D>();
    const auto child2Transform = child2.addComponent<Transform2D>();
    const auto child2Sprite = child2.addComponent<SpriteRenderer>();

    script->loadScript("../assets/Build/libtest.so");
    child2Sprite->loadImage("../assets/Raylib_logo.png");
    child2Sprite->resizeImage(150, 150);
    child2Sprite->initTexture();
    child2Transform->setPosition({-100, -100});
    child2Sprite->getTransform();


    //run engine
    engine.run();

    return 0;
}
