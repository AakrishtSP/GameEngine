#include "Serializer.hpp"


nlohmann::json Serializer::serialize(const GameObject &gameObject) const {
    nlohmann::json jsonObject;
    jsonObject["name"] = gameObject.getName();

    // Serialize components
    nlohmann::json componentsJson;
    for (const auto &[type, component]: gameObject.getComponents()) {
        componentsJson.push_back(serializeComponent(component));
    }
    jsonObject["components"] = componentsJson;

    // Serialize children
    nlohmann::json childrenJson;
    for (const auto &child: gameObject.getChildren()) {
        childrenJson.push_back(serialize(*child));
    }
    jsonObject["children"] = childrenJson;

    return jsonObject;
}

// std::shared_ptr<GameObject> Serializer::deserialize(const nlohmann::json &jsonData) {

//     std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(jsonData["name"].get<std::string>());

//     // Deserialize components
//     for (const auto &componentJson : jsonData["components"])
//     {
//         auto component = deserializeComponent(componentJson, *gameObject);
//         gameObject->addComponent(component);
//     }

//     // Deserialize children
//     for (const auto &childJson : jsonData["children"])
//     {
//         auto child = deserialize(childJson);
//         gameObject->addChild(child);
//     }

//     return gameObject;
// }

nlohmann::json Serializer::serializeComponent(const std::shared_ptr<Component> &component) {
    nlohmann::json jsonComponent;
    // Add component-specific data
    jsonComponent["type"] = component->getType();
    // Add any other component-specific data here
    return jsonComponent;
}

// std::shared_ptr<Component> Serializer::deserializeComponent(const nlohmann::json &jsonData, GameObject &gameObject) {
//     // Create component based on type and set data
//     std::string type = jsonData["type"];
//     std::shared_ptr<Component> component = ComponentFactory::create(type);
//     component->setOwner(&gameObject);
//     // Set any other component-specific data here
//     return component;
// }
