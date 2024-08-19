#include "Engine/GameEngine.hpp"

int main() {
    GameEngine& engine = GameEngine::getInstance();
    engine.init();
    engine.run();

    return 0;
}
