//
// Created by uoyro on 5/15/23.
//

#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_GL_VOXEL_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_GL_VOXEL_H_

#include "glm/glm.hpp"
#include "entt/entt.hpp"

namespace glfw_learn::renderer::gl {

struct Color {
  glm::u8vec4 color;
};

struct Voxel {
  uint32_t division{};
};

/*
 * Система для отрисовки вокселей.
 */
class VoxelSystem {
 public:
  VoxelSystem();
  void operator()(entt::registry &registry);
  ~VoxelSystem();
 private:
  uint32_t vao_{}, vbo_{}, ibo_{}, program_{};
};
}

#endif //GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_GL_VOXEL_H_
