#include "Editor.hpp"

#ifndef RAYGUI_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#endif
#include "raygui.h"


// Initialize the singleton instance
Editor Editor::instance;

// Private constructor to prevent instantiation
Editor::Editor() {};


// Get the singleton instance
Editor &Editor::getInstance() { return instance; }

void Editor::init() {
    root = GameEngine::getInstance().getRoot();
    GuiLoadStyle("../assets/dark.rgs");
}

void Editor::draw() {
    // Draw the hierarchy
    float offsetY = 10.0f;
    float screenHeight = GetScreenHeight();
    Rectangle win = {0, 0, 400, screenHeight};
    GuiWindowBox(win, "Hiararchy");
    drawHierarchy(GameEngine::getInstance().getRoot(), win.x+10.0f, win.y+30.0f, offsetY);

    // // Draw the inspector
    drawInspector(selectedGameObject);

    // Draw the play button
    drawPlayButton();

    // Draw the statistics
    drawStatistics();
}



void Editor::drawPlayButton() {
    float screenWidth = GetScreenWidth();
    // float screenHeight = GetScreenHeight();
    float windowWidth = 70;
    float windowHeight = 35;

    Rectangle win ={(screenWidth-windowWidth)/2, 0, windowWidth, windowHeight};
    // GuiGroupBox(win, "PlayPause");
    // GuiWindowBox(win, "Play");

    Rectangle rect = {win.x, win.y, win.width/2, win.height};
    if (GuiButton(rect, ">")) {
        GameEngine::getInstance().isPlaying = true;
    }
    rect.x += windowWidth/2;
    if (GuiButton(rect, "||")) {
        GameEngine::getInstance().isPlaying = false;
    }
}

void Editor::drawInspector(std::shared_ptr<GameObject> gameObject) {
    if (!gameObject) return; // Check for null pointers

    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();
    float windowWidth = 400;

    Rectangle win = {screenWidth-windowWidth, 0, windowWidth, screenHeight};
    GuiWindowBox(win, "Inspector");
    
    Rectangle rect = {win.x +10 , win.y + 30, win.width - 20, 20};
    // Draw the GameObject's name
    GuiLabel(rect, gameObject->getName().c_str());



    Rectangle right = {win.x + 10, win.y + 60, 180, 20};
    // Draw the GameObject's components
    const auto& components = gameObject->getComponents();
    for (auto it = components.begin(); it != components.end();) {
        if (it->second) {
            rect = it->second->drawInspector(rect);
            ++it;
        }
    }
}

void Editor::drawHierarchy(std::shared_ptr<GameObject> gameObject,float x, float y, float& offsetY) {
    if (!gameObject) return; // Check for null pointers
    // Draw the current GameObject's name
    const float windowWidth = 400;
    float labelWidth = windowWidth - (x + 25) - 20;

    
    // Handle expand/collapse button
    bool isExpanded = expandState[gameObject]; // Get the current state

    if (GuiButton(Rectangle{ x, y, 20, 20 }, isExpanded ? "-" : "+")) {
        expandState[gameObject] = !isExpanded; // Toggle the expand/collapse state
    }

    GuiLabel(Rectangle{ x + 25, y, labelWidth, 20 }, gameObject->getName().c_str());

    offsetY = y + 20; // Update offset for the next node

        // Check if the label was clicked
    if (GuiLabelButton(Rectangle{ x + 25, y, labelWidth, 20 }, gameObject->getName().c_str())) {
        selectedGameObject = gameObject; // Set the selectedGameObject to the clicked one
        // std::cout << "Selected: " << selectedGameObject->getName() << std::endl;
    }

    // If the node is expanded, draw the children
    if (isExpanded) {
        const auto& children = gameObject->getChildren();
        for (const auto& child : children) {
            drawHierarchy(child, x + 20, offsetY, offsetY); // Increase indent for child nodes
        }
    }
}

void Editor::drawStatistics() {
    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();
    float windowWidth = 300;
    float windowHeight = 100;

    Rectangle win = {(screenWidth - windowWidth)/2, screenHeight - windowHeight, windowWidth, windowHeight};
    GuiWindowBox(win, "Statistics");

    // Draw the statistics
    GuiLabel(Rectangle{win.x + 10, win.y + 30, 100, 20}, "Physics Update:");
    GuiLabel(Rectangle{win.x + 10, win.y + 50, 100, 20}, "Render Update:");
    GuiLabel(Rectangle{win.x + 10, win.y + 70, 100, 20}, "Logic Update:");

    // Draw the values
    GuiLabel(Rectangle{win.x + 120, win.y + 30, 100, 20}, std::to_string(GameEngine::getInstance().getPhysicsUpdateInterval()).c_str());
    GuiLabel(Rectangle{win.x + 120, win.y + 50, 100, 20}, std::to_string(GameEngine::getInstance().getRenderUpdateInterval()).c_str());
    GuiLabel(Rectangle{win.x + 120, win.y + 70, 100, 20}, std::to_string(GameEngine::getInstance().getLogicUpdateInterval()).c_str());
}

bool Editor::GuiLabelButton(Rectangle rect, const char *text) {
    bool clicked = false;
    if (GuiButton(rect, "")) {
        clicked = true;
    }
    GuiLabel(rect, text);
    return clicked;
}