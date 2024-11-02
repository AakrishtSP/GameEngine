#pragma once

#include "Components/ComponentFactory.hpp"
#include "GameObject.hpp"
#include "GameEngine.hpp"

#include <iostream>
#include "raylib.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct GameObjectTree {
    std::string name;
    std::shared_ptr<GameObject> gameObject;
    bool expanded = false;
    std::vector<std::shared_ptr<GameObjectTree>> children;
};


class Editor {
public:
    // Get the singleton instance
    static Editor &getInstance();

    // Delete copy constructor and assignment operator to enforce singleton
    Editor(const Editor &) = delete;
    Editor &operator=(const Editor &) = delete;

    void init();

    // Other public methods can be added here
    void draw();

    void setSelectedGameObject(std::shared_ptr<GameObject> gameObject) { selectedGameObject = gameObject; }

    void drawPlayButton();

    void drawInspector(std::shared_ptr<GameObject> gameObject);
    void drawHierarchy(std::shared_ptr<GameObject> gameObject, float x, float y, float& offsetY);

    void drawStatistics();

    static bool GuiLabelButton(Rectangle rect, const char *text) ;

private:
    // Private constructor to prevent instantiation
    Editor();
    GameObjectTree rootTree;
    std::shared_ptr<GameObject> root;
    // Static member for the singleton instance
    static Editor instance;
    std::shared_ptr<GameObject> selectedGameObject;

    std::unordered_map<std::shared_ptr<GameObject>, bool> expandState; // Store expand/collapse state for each GameObject
    bool isHierarchyVisible = true;   //to make the hierarchy box dissapear

    bool isStatsVisible = true;
    bool isInspectorVisible = true;
};
