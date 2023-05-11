#include "ECS/Query.hpp"

Query::Query(std::initializer_list<std::weak_ptr<EntityList>> list)
{
    for (auto& entityList : list)
    {
        _entityLists.push_back(entityList);
    }
}

void Query::AddEntityList(const std::weak_ptr<EntityList>& entityList)
{
    _entityLists.push_back(entityList);
}

void Query::ForEach(const std::function<void(Entity&)>& function) const
{
    for (auto& entityList : _entityLists)
    {
        if (const auto list = entityList.lock())
        {
            list->ForEach(function);
        }
    }
}
