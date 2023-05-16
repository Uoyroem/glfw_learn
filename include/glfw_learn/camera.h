//
// Created by uoyro on 5/14/23.
//

#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_CAMERA_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entt/entt.hpp>

namespace glfw_learn {
struct Camera {
  glm::mat4 projection;
  float speed, sensitivity;
};

void CameraSystem(entt::registry &registry);
}

#endif //GLFW_LEARN_INCLUDE_GLFW_LEARN_CAMERA_H_
