#include "Engine/GameEngine.hpp"


int main() {
    GameEngine& engine = GameEngine::getInstance();
    engine.init();
    engine.setWorldGravity({0, -1000});
    engine.setPhysicsUpdateInterval(1.0f / 120.0f);

    // engine.deserializeScene("../Data/data.json");


    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();
    float half_screen_width = screen_width / 2;
    float half_screen_height = screen_height / 2;

    std::shared_ptr<GameObject> root = engine.getRoot();

    auto rootTransform = root->addComponent<Transform2D>();
    auto rootCollider = root->addComponent<Collider>();
    //Add collider to all bounds of screen
    rootCollider->addCollisionShape(Rectangle{-half_screen_width, 0, 5, screen_height-50});
    rootCollider->addCollisionShape(Rectangle{half_screen_width, 0, 5, screen_height-50});
    rootCollider->addCollisionShape(Rectangle{0, -half_screen_height, screen_width-50, 5});
    rootCollider->addCollisionShape(Rectangle{0, half_screen_height, screen_width-50, 5});


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
    collider->addCollisionShape(Rectangle{0, 0, 150, 150});

    auto rb = child2.addComponent<RigidBody2D>();
    rb->setMoveable(false);
    const auto child2Transform = child2.addComponent<Transform2D>();
    const auto child2Sprite = child2.addComponent<SpriteRenderer>();
    auto collider2 = child2.addComponent<Collider>();

    script->loadScript("../assets/Build/libtest.so");
    child2Sprite->loadImage("../assets/Raylib_logo.png");
    child2Sprite->resizeImage(150, 150);
    child2Sprite->initTexture();
    child2Transform->setPosition({-100, -100});
    child2Sprite->getTransform();
    collider2->addCollisionShape(Rectangle{0, 0, 200, 200});



    //run engine
    engine.run();

    return 0;
}
