#pragma once

#include <memory>

class World;

class SystemBase
{
public:
    SystemBase(const std::shared_ptr<World>& world);

    virtual ~SystemBase() = default;

    virtual void Tick(double deltaTime) = 0;

    [[nodiscard]] World* GetWorld() const;

private:
    std::weak_ptr<World> _world;
};
