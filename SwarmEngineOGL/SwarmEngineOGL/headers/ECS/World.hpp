#pragma once

#include "Components/ComponentStorage.hpp"
#include "EntityTable.hpp"
#include "SystemBase.hpp"

class World : public std::enable_shared_from_this<World>
{
public:
    World() = default;

    ComponentStorage& GetComponentStorage();

    EntityTable& GetEntityTable();

    template <DerivedFromComponent... ComponentTypes>
    Entity* CreateEntity(ComponentTypes&&... components)
    {
        return _entityTable.CreateEntity<ComponentTypes...>(GenerateNewEntityId(),
                                                            std::forward<ComponentTypes>(components)...);
    }

    template <DerivedFromComponent... ComponentTypes>
    void CreateEntities(int count, ComponentTypes&&... components)
    {
        for (int i = 0; i < count; ++i)
        {
            _entityTable.CreateEntity<ComponentTypes...>(GenerateNewEntityId(),
                                                         std::forward<ComponentTypes>(components)...);
        }
    }

    template <DerivedFromComponent... ComponentTypes>
    EntityList* QueryByMatchingType()
    {
        return _entityTable.QueryByMatchingType<ComponentTypes...>();
    }

    template <DerivedFromComponent... ComponentTypes>
    Query QueryByAllTypes()
    {
        return _entityTable.QueryByAllTypes<ComponentTypes...>();
    }

    void RegisterSystem(std::unique_ptr<SystemBase> system);

    void RegisterRendererSystem(std::unique_ptr<SystemBase> system);

    void Tick(double deltaTime) const;

    void Render(double deltaTime) const;

private:
    ComponentStorage _componentStorage;
    EntityTable _entityTable;
    std::vector<std::unique_ptr<SystemBase>> _systems;

    std::vector<std::unique_ptr<SystemBase>> _rendererSystems;

    uint64_t _nextEntityId = 1;

private:
    uint64_t GenerateNewEntityId();
};
