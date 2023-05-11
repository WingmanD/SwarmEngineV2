#include "ECS/SystemBase.hpp"

SystemBase::SystemBase(const std::shared_ptr<World>& world) : _world(world)
{
}

World* SystemBase::GetWorld() const
{
    return _world.lock().get();
}
