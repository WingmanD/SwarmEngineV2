#pragma once

#include "Transform.hpp"

struct ECSTransform : public ECSComponent
{
public:
    Transform InnerTransform;

public:
    ECSTransform() = default;

    ECSTransform(const Transform& transform) : InnerTransform(transform)
    {
    }
};
