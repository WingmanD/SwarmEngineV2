#define STB_IMAGE_IMPLEMENTATION

#undef main

#include <random>
#include <glm/gtx/vector_angle.hpp>

#include "DefaultGameMode.hpp"
#include "DefaultPlayer.hpp"
#include "Engine.hpp"
#include "StaticMesh.hpp"
#include "StaticMeshComponent.hpp"
#include "ECS/ECSRendererWrapper.hpp"
#include "ECS/ECSSystemWrapper.hpp"
#include "ECS/Entity.hpp"
#include "ECS/InlineSystem.hpp"
#include "ECS/World.hpp"
#include "..\headers\ECS\Components\ECSSpawner.hpp"
#include "ECS/Components/ECSNavigation.hpp"
#include "ECS/Components/ECSStaticMesh.hpp"
#include "ECS/Components/ECSTransform.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/EC/CubeObject.hpp"

#define NUM_ENTITIES 1000

void ECSDemo(int argc, char* argv[])
{
    // Engine and scene setup
    Engine* engine = new Engine("ECS Demo", std::filesystem::path(argv[0]).parent_path());
    engine->setGameMode(new DefaultGameMode(engine));

    const auto planeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/plane/plane.obj",
                                                     new Material(new Shader(
                                                         engine->getRuntimePath() / "shaders/blinnPhong/blinnPhong")));
    if (planeImport.empty()) throw std::runtime_error("Plane mesh not found");

    const auto floorObject = new Object(engine);

    StaticMesh* planeMesh = planeImport[0];

    planeMesh->getMaterial()->
               setTextureMap(
                   DIFFUSE,
                   engine->getRuntimePath() / "resources" /
                   "textures/grid.jpeg");
    planeMesh->getMaterial()->setTextureScale({5000.0f, 5000.0f});
    planeMesh->getMaterial()->setAmbient({1.0f, 1.0f, 1.0f});
    planeMesh->getMaterial()->setShininess(25.0f);

    const auto floorMeshComponent = new StaticMeshComponent(planeMesh);
    floorMeshComponent->attachTo(floorObject);

    floorObject->setScale({
        5000.0f, 1.0f, 5000.0f
    });
    engine->getScene()->addObject(floorObject);

    const auto skyboxImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/skybox/skybox.obj",
                                                      new Material(
                                                          new Shader(engine->getRuntimePath() / "shaders/sky/sky")));
    if (skyboxImport.empty()) throw std::runtime_error("Skybox mesh not found");

    const auto skyboxObject = new Object(engine);

    StaticMesh* skyboxMesh = skyboxImport[0];
    const auto skyboxMeshComponent = new StaticMeshComponent(skyboxMesh);
    skyboxMeshComponent->attachTo(skyboxObject);
    skyboxObject->setScale({
        50.0f, 50.0f, 50.0f
    });
    engine->getScene()->addObject(skyboxObject);

    const auto player = new DefaultPlayer(engine);
    const auto playerLight = new Light({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 2.5f);
    engine->getScene()->addLight(playerLight);
    playerLight->attachTo(player);
    engine->getScene()->addObject(player);
    engine->possess(player);

    const auto light = new Light({0.0f, 25.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 15.0f);
    engine->getScene()->addLight(light);
    light->attachTo(floorObject);

    engine->getRenderer()->setUnlit(false);
    engine->setCursorVisible(false);

    // ECS Demo
    const auto cubeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/cube/cube.obj",
                                                    new Material(new Shader(
                                                        engine->getRuntimePath() / "shaders/blinnPhong/blinnPhong")));
    if (cubeImport.empty()) throw std::runtime_error("Cube mesh not found");
    StaticMesh* cubeMesh = cubeImport[0];
    if (cubeMesh == nullptr) throw std::runtime_error("Cube mesh is null");

    const auto f16Import = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/f16/f16.obj",
                                                      new Material(new Shader(
                                                          engine->getRuntimePath() / "shaders/blinnPhong/blinnPhong")));
    if (f16Import.empty()) throw std::runtime_error("Sphere mesh not found");
    StaticMesh* f16Mesh = f16Import[0];
    if (f16Mesh == nullptr) throw std::runtime_error("Sphere mesh is null");

    auto world = engine->GetWorld();

    // create factory entities
    world->CreateEntity<ECSTransform, ECSStaticMesh, ECSSpawner>({
         Transform(
             {2.0f, 0.1f, 0.0f},
             {0.0f, 0.0f, 0.0f},
             {0.1f, 0.1f, 0.1f})
     },
     {cubeMesh},
     {
         25,
         1.5f,
         {
             Transform(
                 {0.0f, 0.5f, 0.0f},
                 {0.0f, 0.0f, 0.0f},
                 {0.1f, 0.1f, 0.1f})
         },
         f16Mesh
     });

    world->CreateEntity<ECSTransform, ECSStaticMesh, ECSSpawner>({
         Transform(
             {2.0f, 0.1f, 4.0f},
             {0.0f, 0.0f, 0.0f},
             {0.1f, 0.1f, 0.1f})
     },
     {cubeMesh},
     {
         10,
         3.0f,
         {
             Transform(
                 {0.0f, 0.5f, 0.0f},
                 {0.0f, 0.0f, 0.0f},
                 {0.1f, 0.1f, 0.1f})
         },
         f16Mesh
     });

    InlineSystem<ECSTransform, ECSSpawner>::Create(world, [world](Entity& entity, double deltaTime)
    {
        const auto transform = entity.Get<ECSTransform>();
        const auto spawner = entity.Get<ECSSpawner>();

        if (glfwGetTime() - spawner->LastSpawnTime > spawner->SpawnInterval
            && spawner->SpawnCount < spawner->MaxSpawnCount)
        {
            spawner->LastSpawnTime = glfwGetTime();
            world->CreateEntity<ECSTransform, ECSStaticMesh, ECSNavigation>({
                Transform(
                    transform->InnerTransform.
                    getLocation() + spawner->
                    SpawnOffset.getLocation(),
                    {0.0f, 0.0f, 0.0f},
                    {0.5f, 0.5f, 0.5f})
            },
            {spawner->Mesh},
            {
                {5.0f, 0.1f, 0.0f}, 1.0f
            });
            spawner->SpawnCount++;
        }
    });

    InlineSystem<ECSTransform, ECSNavigation>::Create(world, [](Entity& entity, double deltaTime)
    {
        const auto transform = entity.Get<ECSTransform>();
        const auto navigation = entity.Get<ECSNavigation>();

        const float distance = glm::distance(navigation->TargetPosition, transform->InnerTransform.getLocation());
        if (distance > 0.1f)
        {
            const glm::vec3 direction = normalize(
                navigation->TargetPosition - transform->InnerTransform.getLocation());


            const float pitch = transform->InnerTransform.getRotation().z;
            const float yaw = transform->InnerTransform.getRotation().y;

            glm::vec3 currentDirection;
            currentDirection.x = cos(pitch) * cos(yaw);
            currentDirection.y = sin(pitch);
            currentDirection.z = cos(pitch) * sin(yaw);
            currentDirection = normalize(currentDirection);

            const float sign = asin(direction.y) - pitch;

            transform->InnerTransform.move(direction * static_cast<float>(navigation->Speed * deltaTime));
            transform->InnerTransform.rotate({0.0f, sign * angle(currentDirection, direction), 0.0f});
        }
    });

    engine->run();

    delete engine;
}

int main(int argc, char* argv[])
{
    ECSDemo(argc, argv);

    return 0;
}
