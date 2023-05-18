//
// Created by uoyro on 5/15/23.
//

#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_GL_VOXEL_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_GL_VOXEL_H_

#include "glm/glm.hpp"
#include "entt/entt.hpp"
#include <unordered_map>
#include <bitset>

namespace glfw_learn::renderer::gl {
constexpr int VOXEL_FRONT = 1;
constexpr int VOXEL_BACK = 1 << 1;
constexpr int VOXEL_TOP = 1 << 2;
constexpr int VOXEL_BOTTOM = 1 << 3;
constexpr int VOXEL_RIGHT = 1 << 4;
constexpr int VOXEL_LEFT = 1 << 5;
constexpr int VOXEL_ALL = VOXEL_FRONT | VOXEL_BACK | VOXEL_TOP | VOXEL_BOTTOM | VOXEL_RIGHT | VOXEL_LEFT;

using VoxelFacesBitset = std::bitset<6>;

struct Color {
  glm::u8vec4 color;
};

struct Voxel {
  VoxelFacesBitset faces{};
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
  uint32_t vao_{}, vbo_{}, program_{};
  std::unordered_map<VoxelFacesBitset, uint32_t> ibo_map_;

  uint32_t GetIbo(VoxelFacesBitset faces);
};
}

#endif //GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_GL_VOXEL_H_
