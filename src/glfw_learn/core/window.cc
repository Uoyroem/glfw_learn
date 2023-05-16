//
// Created by uoyro on 5/15/23.
//

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>
#include <random>

#include "glfw_learn/renderer/gl/gl.h"
#include "glfw_learn/core/window.h"
#include <entt/entt.hpp>
#include "glfw_learn/camera.h"
#include "glfw_learn/renderer/gl/voxel.h"
#include "glfw_learn/transform.h"

namespace glfw_learn::core {

float delta_time;
GLFWmonitor *monitor = nullptr;
const GLFWvidmode *mode = nullptr;
GLFWwindow *window = nullptr;
int width, height;

extern void KeyCallback(GLFWwindow *, int, int, int, int);
extern void CursorPosCallback(GLFWwindow *, double, double);

void Initialize();
void SetWindowHints();

void Run() {
  using namespace glfw_learn;
  using namespace glfw_learn::renderer::gl;

  Initialize();

  entt::registry registry;
  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {
      auto voxel = registry.create();
      registry.emplace<Voxel>(voxel);
      registry.emplace<Transform>(voxel, glm::vec3(i * 2, 0, j * 2));
      registry.emplace<Color>(voxel, glm::u8vec4(rand() % 70, 255 - rand() % 50, 0, 255));
    }
  }

  auto camera = registry.create();
  registry.emplace<Camera>(camera, glm::perspective(45.0f, core::GetAspect(), 0.01f, 500.0f), 10, 25);
  auto &camera_transform = registry.emplace<Transform>(camera);
  auto &camera_direction = registry.emplace<Direction>(camera);
  registry.emplace<LookAtDirectionTag>(camera);

  glfw_learn::renderer::gl::VoxelSystem voxel_system;

  while (!glfwWindowShouldClose(window)) {
    auto start = static_cast<float>(glfwGetTime());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // SYSTEMS
    glfw_learn::TransformSystem(registry);
    glfw_learn::DirectionSystem(registry);
    glfw_learn::LookAtDirectionSystem(registry);
    glfw_learn::CameraSystem(registry);
    voxel_system(registry);

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

  renderer::gl::InitializeOpenGL();

  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, width, height);
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