#pragma once

#include <memory>
#include "Components/ECSComponent.hpp"
#include "Components/ComponentStorage.hpp"

class Entity
{
public:
    Entity() = default;

    explicit Entity(uint64_t id)
    {
        _id = id;
    }

    template <DerivedFromComponent ComponentType>
    void Add(ComponentType&& component)
    {
        ComponentType* comp = ComponentStorage::Get().CreateComponent(std::forward<ComponentType>(component));
        _components.emplace_back(std::unique_ptr<ECSComponent, ComponentDeleter>(comp));
    }

    template <DerivedFromComponent... ComponentTypes>
    void Add(ComponentTypes&&... components)
    {
        (Add(std::forward<ComponentTypes>(components)), ...);
    }

    template <DerivedFromComponent ComponentType>
    ComponentType* Get()
    {
        for (auto& component : _components)
        {
            if (auto comp = dynamic_cast<ComponentType*>(component.get()))
            {
                return comp;
            }
        }

        return nullptr;
    }

    template <DerivedFromComponent... ComponentTypes>
    static Entity CreateEntity(uint64_t id, ComponentTypes&&... components)
    {
        Entity newEntity(id);
        newEntity.Add(std::forward<ComponentTypes>(components)...);

        return newEntity;
    }

    [[nodiscard]] bool IsValid() const
    {
        return _id != 0;
    }

    void Destroy()
    {
        _id = 0;
    }

private:
    uint64_t _id = 0;

    std::vector<ComponentUniquePtr> _components{};
};
