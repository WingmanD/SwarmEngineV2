#pragma once

#include <initializer_list>
#include <memory>
#include "EntityList.hpp"

class Query
{
public:
    Query() = default;

    Query(std::initializer_list<std::weak_ptr<EntityList>> list);

    void AddEntityList(const std::weak_ptr<EntityList>& entityList);

    void ForEach(const std::function<void(Entity&)>& function) const;

private:
    std::vector<std::weak_ptr<EntityList>> _entityLists;
};
