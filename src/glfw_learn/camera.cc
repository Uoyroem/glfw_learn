//
// Created by uoyro on 5/14/23.
//

#include "glfw_learn/camera.h"
#include "glfw_learn/core/input.h"
#include "glfw_learn/core/time.h"
#include "glfw_learn/transform.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace glfw_learn {
void CameraSystem(entt::registry &registry) {
  using namespace core;

  static float pitch = 0, yaw = -90.0f;

  auto [camera, transform, direction] =
      registry.get<Camera, Transform, Direction>(registry.group<Camera, Transform, Direction>().back());

  auto x = static_cast<float>(IsKeyPressed(GLFW_KEY_D) - IsKeyPressed(GLFW_KEY_A));
  auto y = static_cast<float>(IsKeyPressed(GLFW_KEY_SPACE) - IsKeyPressed(GLFW_KEY_LEFT_SHIFT));
  auto z = static_cast<float>(IsKeyPressed(GLFW_KEY_W) - IsKeyPressed(GLFW_KEY_S));

  auto delta_time = core::GetDeltaTime();

  transform.position +=
      (direction.right * x + direction.up * y + direction.forward * z) * camera.speed * delta_time;

  auto &mouse_ox = GetXMouseOffset();
  auto &mouse_oy = GetYMouseOffset();

  if (static_cast<bool>(mouse_ox) || static_cast<bool>(mouse_oy)) {
    pitch += mouse_oy * camera.sensitivity * delta_time;
    yaw -= mouse_ox * camera.sensitivity * delta_time;

    direction.forward.x = cosf(glm::radians(pitch)) * cosf(glm::radians(yaw));
    direction.forward.y = sinf(glm::radians(pitch));
    direction.forward.z = cosf(glm::radians(pitch)) * sinf(glm::radians(yaw));
  }

  mouse_ox = 0;
  mouse_oy = 0;
}
}

