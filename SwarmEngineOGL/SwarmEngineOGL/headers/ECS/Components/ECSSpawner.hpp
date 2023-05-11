#pragma once

#include "ECSComponent.hpp"
#include "StaticMesh.hpp"
#include "Transform.hpp"

struct ECSSpawner : public ECSComponent
{
public:
    float SpawnInterval = 1.5f;
    float LastSpawnTime = 0.0f;
    int SpawnCount = 0;
    int MaxSpawnCount = 25;

    StaticMesh* Mesh = nullptr;

    Transform SpawnOffset = Transform(glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));

public:
    ECSSpawner() = default;

    ECSSpawner(int spawnCount, float spawnTimer, const Transform& spawnOffset, StaticMesh* mesh) :
        SpawnInterval(spawnTimer), MaxSpawnCount(spawnCount), Mesh(mesh), SpawnOffset(spawnOffset)
    {
    }
};
