#pragma once


#include "EntityList.hpp"
#include "Type.hpp"
#include "Query.hpp"
#include "Entity.hpp"

class EntityTable
{
public:
    explicit EntityTable() = default;

    template <DerivedFromComponent... ComponentTypes>
    Entity* CreateEntity(uint64_t id, ComponentTypes&&... components)
    {
        if (auto entityList = GetOrCreateEntityList<ComponentTypes...>())
        {
            return entityList->Add(
                Entity::CreateEntity(id, std::forward<ComponentTypes>(components)...));
        }

        // todo log
        return nullptr;
    }

    EntityList* GetEntityListForType(const Type& type);

    template <DerivedFromComponent... ComponentTypes>
    EntityList* QueryByMatchingType()
    {
        auto type = Type::MakeCompositeType<Entity, ComponentTypes...>();

        if (_entityLists.contains(type))
        {
            return _entityLists[type];
        }

        return nullptr;
    }

    template <DerivedFromComponent... ComponentTypes>
    Query QueryByAllTypes() const
    {
        auto type = Type::MakeCompositeType<Entity, ComponentTypes...>();

        Query query;

        for (auto& entityList : _entityLists)
        {
            if (entityList.first.HasAll<ComponentTypes...>())
            {
                query.AddEntityList(std::weak_ptr(entityList.second));
            }
        }

        return query;
    }

private:
    std::map<Type, std::shared_ptr<EntityList>> _entityLists{};

private:
    template <DerivedFromComponent... ComponentTypes>
    EntityList* GetOrCreateEntityList()
    {
        auto type = Type::MakeCompositeType<Entity, ComponentTypes...>();

        if (!_entityLists.contains(type))
        {
            _entityLists[type] = std::make_shared<EntityList>();
        }

        return _entityLists[type].get();
    }
};
