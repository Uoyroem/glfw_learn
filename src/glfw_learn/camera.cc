//
// Created by uoyro on 5/14/23.
//

#include "glfw_learn/core/engine.h"
#include "glfw_learn/camera.h"

#include "glfw_learn/transform.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace glfw_learn {
void CameraSystem(entt::registry& registry) {
  using namespace core;

  auto engine = Engine::Get();

  auto [camera, transform, direction] =
      registry.get<Camera, Transform, Direction>(
          registry.group<Camera, Transform, Direction>().back());

  auto x =
      static_cast<float>(engine->GetKey(GLFW_KEY_D) - engine->GetKey(GLFW_KEY_A));
  auto y = static_cast<float>(engine->GetKey(GLFW_KEY_SPACE) -
                              engine->GetKey(GLFW_KEY_LEFT_SHIFT));
  auto z = static_cast<float>(engine->GetKey(GLFW_KEY_W) -
                              engine->GetKey(GLFW_KEY_S));

  auto delta_time = engine->GetDeltaTime();

  transform.position += (direction.right * x + glm::vec3(0, 1.0f, 0) * y +
                         direction.forward * z) *
                        camera.speed * delta_time;

  float mouse_x = engine->GetMouseX();
  float mouse_y = engine->GetMouseY();

  static bool initialized = false;
  static float last_mouse_x = 0, last_mouse_y = 0;

  float mouse_ox = last_mouse_x - mouse_x;
  float mouse_oy = last_mouse_y - mouse_y;

  if ((mouse_ox || mouse_oy) && initialized) {
    direction.euler_angles.x -= mouse_oy * camera.sensitivity * delta_time;
    direction.euler_angles.x =
        glm::clamp(direction.euler_angles.x, -89.0f, 89.0f);
    direction.euler_angles.y += mouse_ox * camera.sensitivity * delta_time;
  }

  last_mouse_x = mouse_x;
  last_mouse_y = mouse_y;

  if (!initialized) {
    initialized = true;
  }
}
}  // namespace glfw_learn
