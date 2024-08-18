#include "Engine/GameEngine.hpp"

int main() {
    GameEngine& engine = GameEngine::getInstance();
    engine.run();

    return 0;
}
