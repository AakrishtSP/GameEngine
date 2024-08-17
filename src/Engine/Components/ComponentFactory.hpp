#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include "../Component.hpp"

class ComponentFactory {
public:
    using ComponentCreator = std::function<std::shared_ptr<Component>()>;

    static ComponentFactory& getInstance() {
        static ComponentFactory instance;
        return instance;
    }

    template <typename T>
    void registerComponent(const std::string& componentType) {
        creators[componentType] = []() { return std::make_shared<T>(); };
    }

    std::shared_ptr<Component> create(const std::string& componentType) const {
        auto it = creators.find(componentType);
        if (it != creators.end()) {
            return it->second();
        }
        std::cerr << "Component type not found: " << componentType << std::endl;
        return nullptr;
    }

private:
    std::unordered_map<std::string, ComponentCreator> creators;

    ComponentFactory() = default;
    ComponentFactory(const ComponentFactory&) = delete;
    ComponentFactory& operator=(const ComponentFactory&) = delete;
};

