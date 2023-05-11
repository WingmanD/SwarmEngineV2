#pragma once

#include <memory>

struct ECSComponent
{
    ECSComponent() = default;

    virtual ~ECSComponent() = default;

    explicit ECSComponent(bool isValid);

    [[nodiscard]] bool IsValid() const;

    virtual void Destroy();

private:
    bool _isValid = false;
};

struct ComponentDeleter
{
    void operator()(ECSComponent* component) const
    {
        component->Destroy();
    }
};

using ComponentUniquePtr = std::unique_ptr<ECSComponent, ComponentDeleter>;

template <typename T>
concept DerivedFromComponent = std::is_base_of_v<ECSComponent, T>;
