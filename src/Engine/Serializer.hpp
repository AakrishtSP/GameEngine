#pragma once
#include "Base.hpp"
#include <nlohmann/json.hpp> 

class GameObject;
class Component;

class Serializer
{
public:
    // Serialize a game object to a JSON object
    [[nodiscard]] nlohmann::json serialize(const GameObject &gameObject) const;
    static static nlohmann::json serializeComponent(const std::shared_ptr<Component> &component);

    // Deserialize a game object from a JSON object
    std::shared_ptr<GameObject> deserialize(const nlohmann::json &jsonData);

private:
    // Helper function to serialize a component
    [[nodiscard]] nlohmann::json serializeComponent(const std::shared_ptr<Component> &component) const;

    // Helper function to deserialize a component
    std::shared_ptr<Component> deserializeComponent(const nlohmann::json &jsonData, GameObject &gameObject);
};
