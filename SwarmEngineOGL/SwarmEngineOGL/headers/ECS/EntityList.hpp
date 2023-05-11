#pragma once

#include "Entity.hpp"
#include "DynamicCollection.hpp"

struct EntityList : public DynamicCollection<Entity>
{
};
