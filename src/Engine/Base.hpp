#pragma once

#include "raylib.h"
#include "GameObject.hpp"
#include "Component.hpp"
#include "Components/Components.hpp"
#include "GameEngine.hpp"
#include "Editor.hpp"
#include <nlohmann/json.hpp>

// class EmptyComponent : public Component
// {
// public:
//     EmptyComponent() {}
//     void update() override { std::cout << "EmptyComponent update" << std::endl; }
// };