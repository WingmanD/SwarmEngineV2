#include "ECS/EntityTable.hpp"

EntityList* EntityTable::GetEntityListForType(const Type& type)
{
    if (!_entityLists.contains(type))
    {
        _entityLists[type] = std::make_unique<EntityList>();
    }

    return _entityLists[type].get();
}
