#pragma once
#include "ECSComponent.hpp"

class StaticMesh;

struct ECSStaticMesh : public ECSComponent
{
public:
    StaticMesh* Mesh = nullptr;

public:
    ECSStaticMesh() = default;

    ECSStaticMesh(StaticMesh* mesh) : Mesh(mesh)
    {
    }
};
