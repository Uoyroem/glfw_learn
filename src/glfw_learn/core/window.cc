//
// Created by uoyro on 5/15/23.
//

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>
#include <random>
#include <entt/entt.hpp>
#include <filesystem>
#include <iostream>

#include "glfw_learn/renderer/gl.h"
#include "glfw_learn/core/window.h"
#include "glfw_learn/core/resource.h"
#include "glfw_learn/camera.h"
#include "glfw_learn/renderer/voxel.h"
#include "glfw_learn/transform.h"
#include "glfw_learn/renderer/shader.h"

namespace glfw_learn::core {

float delta_time;

static GLFWmonitor *monitor = nullptr;
static const GLFWvidmode *mode = nullptr;
static GLFWwindow *window = nullptr;
static int width, height;

extern void KeyCallback(GLFWwindow *, int, int, int, int);
extern void CursorPosCallback(GLFWwindow *, double, double);

void Initialize();
void SetWindowHints();



void Run(int argc, char **argv) {
  using namespace glfw_learn;
  using namespace glfw_learn::renderer;

  Initialize();
  entt::registry registry;
  auto voxel1 = registry.create();
  registry.emplace<Voxel>(voxel1, 1U);
  registry.emplace<Transform>(voxel1, glm::vec3(-1.0f, -1.0f, 1.0f));
  registry.emplace<Color>(voxel1, glm::u8vec4(0, 0, 255, 0));

  auto voxel2 = registry.create();
  registry.emplace<Voxel>(voxel2);
  registry.emplace<Transform>(voxel2, glm::vec3{2.0f, 0, 0});
  registry.emplace<Color>(voxel2, glm::u8vec4(255, 0, 0, 0));

  auto camera = registry.create();
  registry.emplace<Camera>(
      camera, glm::perspective(45.0f, core::GetAspect(), 0.01f, 500.0f), 30.0f,
      50.0f);
  registry.emplace<Transform>(camera);
  registry.emplace<Direction>(camera);
  registry.emplace<LookAtDirectionTag>(camera);  

  VoxelRenderingSystem voxel_rendering_system{registry};
  ResourceSystem resource_system{std::filesystem::path(argv[0]).parent_path() /
                                 "resources"};

  while (!glfwWindowShouldClose(window)) {
    auto start = static_cast<float>(glfwGetTime());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    resource_system(registry);
    ShaderCompilingSystem(registry);
    ProgramLinkingSystem(registry);
    VoxelTransformSystem(registry);
    TransformSystem(registry);
    DirectionSystem(registry);
    LookAtDirectionSystem(registry);
    CameraSystem(registry);
    voxel_rendering_system();

    delta_time = static_cast<float>(glfwGetTime()) - start;
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}

float GetAspect() {
  return static_cast<float>(width) / static_cast<float>(height);
}

int GetWidth() {
  return width;
}

int GetHeight() {
  return height;
}

void Initialize() {
  glfwInit();
  monitor = glfwGetPrimaryMonitor();
  mode = glfwGetVideoMode(monitor);

  SetWindowHints();

  window = glfwCreateWindow(mode->width / 2, mode->height / 2, "GLFW Learn",
                            nullptr, nullptr);
  glfwMakeContextCurrent(window);

  glfwGetFramebufferSize(window, &width, &height);

  glfwSetKeyCallback(window, KeyCallback);
  glfwSetCursorPosCallback(window, CursorPosCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  renderer::InitializeOpenGL();
}

void SetWindowHints() {
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
}
}