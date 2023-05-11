#pragma once

#include <random>
#include <glm/vec3.hpp>

#include "ECSComponent.hpp"

struct ECSNavigation : public ECSComponent
{
public:
    glm::vec3 TargetPosition = glm::vec3(0.0f);
    float Speed = 1.0f;

public:
    ECSNavigation() = default;

    ECSNavigation(const glm::vec3& targetPosition, float speed) : Speed(speed)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution distribution(0.0f, 2.0f);
        std::uniform_real_distribution distributionUp(0.0f, 0.5f);

        TargetPosition = targetPosition + glm::vec3(distribution(gen), distributionUp(gen), distribution(gen));
    }
};
