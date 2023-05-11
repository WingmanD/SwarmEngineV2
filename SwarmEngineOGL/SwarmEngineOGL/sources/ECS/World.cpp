#include "ECS/World.hpp"

ComponentStorage& World::GetComponentStorage()
{
    return _componentStorage;
}

EntityTable& World::GetEntityTable()
{
    return _entityTable;
}

void World::RegisterSystem(std::unique_ptr<SystemBase> system)
{
    _systems.emplace_back(std::move(system));
}

void World::RegisterRendererSystem(std::unique_ptr<SystemBase> system)
{
    _rendererSystems.emplace_back(std::move(system));
}

void World::Tick(double deltaTime) const
{
    for (auto& system : _systems)
    {
        system->Tick(deltaTime);
    }
}

void World::Render(double deltaTime) const
{
    for (auto& system : _rendererSystems)
    {
        system->Tick(deltaTime);
    }
}

uint64_t World::GenerateNewEntityId()
{
    return _nextEntityId++;
}
