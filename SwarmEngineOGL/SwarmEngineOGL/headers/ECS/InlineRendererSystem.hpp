#pragma once

#include "World.hpp"
#include "SystemBase.hpp"
#include <utility>

template <typename... ComponentTypes>
class InlineRendererSystem : public SystemBase
{
public:
    InlineRendererSystem(const std::shared_ptr<World>& world,
                         std::function<void(Entity&, double)> processEntityFunction)
        : SystemBase(
              world), _processEntityFunction(std::move(processEntityFunction))
    {
    }

    virtual void Tick(double deltaTime) override
    {
        Query query = GetWorld()->QueryByAllTypes<ComponentTypes...>();
        query.ForEach([this, deltaTime](Entity& entity)
        {
            _processEntityFunction(entity, deltaTime);
        });
    }

    static InlineRendererSystem*
    Create(const std::shared_ptr<World>& world, std::function<void(Entity&, double)> processEntityFunction)
    {
        if (world == nullptr)
        {
            // todo log
            return nullptr;
        }

        auto newSystem = new InlineRendererSystem(world, processEntityFunction);

        std::unique_ptr<SystemBase> systemBase = std::unique_ptr<InlineRendererSystem>(newSystem);

        world->RegisterRendererSystem(std::move(systemBase));

        return newSystem;
    }

private:
    std::function<void(Entity&, double)> _processEntityFunction;
};
