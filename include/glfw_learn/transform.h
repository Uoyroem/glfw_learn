//
// Created by uoyro on 5/16/23.
//

#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_TRANSFORM_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_TRANSFORM_H_

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace glfw_learn {
struct Transform {
  glm::vec3 position{};
  glm::mat4 transform{1.0f};
};

struct Direction {
  glm::vec3 forward{0, 0, -1.0f};
  glm::vec3 right{};
  glm::vec3 up{};
};

struct LookAtDirectionTag {};

void LookAtDirectionSystem(entt::registry &registry);
void DirectionSystem(entt::registry &registry);
void TransformSystem(entt::registry &registry);
}

#endif //GLFW_LEARN_INCLUDE_GLFW_LEARN_TRANSFORM_H_
