﻿#include "Engine.hpp"


#include <iostream>
#include <glad/glad.h>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GameMode.hpp"
#include "StaticMesh.hpp"
#include "ECS/InlineRendererSystem.hpp"
#include "ECS/Components/ECSStaticMesh.hpp"
#include "ECS/Components/ECSTransform.hpp"

Engine::Engine(const std::string& title, std::filesystem::path runtimePath) : runtimePath(std::move(runtimePath)),
                                                                              _title(title)
{
    soundEngine = new SoundEngine();
    initOpenGL();
    initUI();

    scene = new Scene();
    renderer = new Renderer(scene);
    widgetManager = new WidgetManager();
    _world = std::make_shared<World>();
}

void Engine::run()
{
    InlineRendererSystem<ECSTransform, ECSStaticMesh>::Create(_world, [](Entity& entity, double deltaTime)
    {
        const auto transformComponent = entity.Get<ECSTransform>();
        const auto meshComponent = entity.Get<ECSStaticMesh>();

        if (meshComponent->Mesh != nullptr)
        {
            meshComponent->Mesh->draw(transformComponent->InnerTransform);
        }
    });

    double lastFrame = 0.0;
    double deltaTime = 0.0;
    while (glfwWindowShouldClose(windowOGL) == false)
    {
        const double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        auto start = std::chrono::high_resolution_clock::now();

        if (!bPaused && scene)
        {
            scene->tick(deltaTime);
            _world->Tick(deltaTime);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        _lastTickDuration = elapsed.count();

        if (renderer) renderer->render();
        _world->Render(deltaTime);

        ImGui::SetNextWindowSize({static_cast<float>(width), static_cast<float>(height)});
        if (widgetManager) widgetManager->drawWidgets();

        glfwSwapBuffers(windowOGL);
    }
}

void Engine::possess(Player* player)
{
    this->activePlayer = player;
}

void Engine::setCursorVisible(bool bVisible) const
{
    glfwSetInputMode(windowOGL, GLFW_CURSOR, bVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Engine::initOpenGL()
{
    glfwInit();
    gladLoadGL();

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    windowOGL = glfwCreateWindow(width, height, _title.c_str(), nullptr, nullptr);
    if (!windowOGL)
        throw std::runtime_error("Failed to Create OpenGL Context");

    glfwMakeContextCurrent(windowOGL);
    glfwSetWindowUserPointer(windowOGL, this);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        throw std::runtime_error("Failed to initialize GLAD");

    glfwSetFramebufferSizeCallback(windowOGL, [](GLFWwindow* window, int newWidth, int newHeight)
    {
        static_cast<Engine*>(glfwGetWindowUserPointer(window))->framebufferSizeCallback(window, newWidth, newHeight);
    });

    glfwSetKeyCallback(windowOGL, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        static_cast<Engine*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(windowOGL, [](GLFWwindow* window, double x, double y)
    {
        static_cast<Engine*>(glfwGetWindowUserPointer(window))->cursorCallback(window, x, y);
    });

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.0f, 1);

    glfwSwapInterval(0);
}

void Engine::initUI() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(windowOGL, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Engine::framebufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
    this->width = newWidth;
    this->height = newHeight;

    glViewport(0, 0, width, height);

    if (gameMode) gameMode->onWindowSizeChange(width, height);
    if (activePlayer) activePlayer->onWindowSizeChange(width, height);
}

void Engine::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    cursorPos.x = static_cast<float>(xpos);
    cursorPos.y = static_cast<float>(ypos);

    if (gameMode) gameMode->onMouseMove(xpos, ypos);
    if (activePlayer) activePlayer->onMouseMove(xpos, ypos);
}

void Engine::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (gameMode) gameMode->onScroll(xoffset, yoffset);
    if (activePlayer) activePlayer->onScroll(xoffset, yoffset);
}

void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (gameMode) gameMode->onKey(key, scancode, action, mods);
    if (activePlayer) activePlayer->onKey(key, scancode, action, mods);
}


void Engine::setPaused(bool paused)
{
    bPaused = paused;
    if (gameMode) paused ? gameMode->pause() : gameMode->resume();
}

double Engine::getLastTickDuration() const
{
    return _lastTickDuration;
}

Engine::~Engine()
{
    delete scene;
    delete renderer;
    delete widgetManager;
    delete soundEngine;
    delete gameMode;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}
