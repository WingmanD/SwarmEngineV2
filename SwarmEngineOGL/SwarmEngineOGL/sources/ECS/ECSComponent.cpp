#include "ECS/Components/ECSComponent.hpp"

ECSComponent::ECSComponent(bool isValid) : _isValid(isValid)
{
}

bool ECSComponent::IsValid() const
{
    return _isValid;
}

void ECSComponent::Destroy()
{
    _isValid = false;
}
