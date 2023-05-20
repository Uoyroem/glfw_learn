//
// Created by uoyro on 5/16/23.
//

#include "glfw_learn/transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace glfw_learn {

void LookAtDirectionSystem(entt::registry &registry) {
  auto view = registry.view<Transform, Direction, LookAtDirectionTag>();

  for (auto [_, transform, direction] : view.each()) {
    transform.transform =
        glm::lookAt(transform.position, transform.position + direction.forward, glm::vec3(0, 1.0f, 0));
  }
}


void DirectionSystem(entt::registry &registry) {
  auto view = registry.view<Direction>();

  for (auto [_, direction] : view.each()) {
    direction.forward = glm::normalize(glm::vec3(
        glm::eulerAngleYXZ(glm::radians(direction.euler_angles.y), glm::radians(direction.euler_angles.x),
                           glm::radians(direction.euler_angles.z)) * glm::vec4(0, 0, 1.0f, 0)));
    direction.right = glm::normalize(glm::cross(direction.forward, glm::vec3(0, 1.0f, 0)));
    direction.up = glm::normalize(glm::cross(direction.right, direction.forward));
  }
}


void TransformSystem(entt::registry &registry) {
  auto view = registry.view<Transform>(entt::exclude<Direction>);

  for (auto [_, transform] : view.each()) {
    transform.transform = glm::scale(glm::mat4(1.0f), transform.scale);
    transform.transform =
        glm::translate(transform.transform, transform.position); 
  }
}
}
