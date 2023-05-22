//
// Created by uoyro on 5/15/23.
//

#include <glad/gl.h>

#include <entt/entt.hpp>
#include <filesystem>
#include <iostream>
#include <random>

#include "glfw_learn/camera.h"
#include "glfw_learn/core/engine.h"
#include "glfw_learn/transform.h"

namespace glfw_learn::core {

void Engine::Initialize(const std::string_view& argv0) {
  if (!instance_) {
    instance_ = std::shared_ptr<Engine>(new Engine(argv0));
  }
}

Engine::Engine(const std::string_view& argv0)
    : resource_manager_{std::make_shared<ResourceManager>(
          std::filesystem::path(argv0).parent_path() / "resources")} {
  glfwInit();
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

  auto width = vidmode->width / 2;
  auto height = vidmode->height / 2;

  window_ = glfwCreateWindow(width, height, "GLFW Learn", nullptr, nullptr);

  glfwMakeContextCurrent(window_);

  glfwGetFramebufferSize(window_, &width, &height);

  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  gladLoadGL(glfwGetProcAddress);
   

  glDebugMessageCallback(OpenGLDebugCallback_, nullptr);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glViewport(0, 0, width, height);
}

Engine::~Engine() noexcept { glfwTerminate(); }

void Engine::Run() {
  using namespace renderer;

  entt::registry registry;
  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {
      auto voxel = registry.create();
      registry.emplace<Voxel>(voxel);
      registry.emplace<Transform>(voxel, glm::vec3(i * 2, 0, j * 2));
      registry.emplace<Color>(voxel, glm::u8vec4(0, 255 - rand() % 50, 0, 255));
    }
  }

  int width, height;
  glfwGetFramebufferSize(window_, &width, &height);

  auto camera = registry.create();
  auto projection =
      glm::perspective(45.0f, static_cast<float>(width) / height, 0.01f, 30.0f);
  registry.emplace<Camera>(camera, projection, 30.0f, 50.0f);
  registry.emplace<Transform>(camera);
  registry.emplace<Direction>(camera);
  registry.emplace<LookAtDirectionTag>(camera);

  VoxelRenderingSystem voxel_rendering_system;

  while (!glfwWindowShouldClose(window_)) {
    auto start = static_cast<float>(glfwGetTime());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    VoxelTransformSystem(registry);
    TransformSystem(registry);
    DirectionSystem(registry);
    LookAtDirectionSystem(registry);
    CameraSystem(registry);
    voxel_rendering_system(registry);

    delta_time_ = static_cast<float>(glfwGetTime()) - start;
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void Engine::OpenGLDebugCallback_(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar* message,
                                  const void* user_param) {
  std::cout << message << std::endl;
}
}  // namespace glfw_learn::core