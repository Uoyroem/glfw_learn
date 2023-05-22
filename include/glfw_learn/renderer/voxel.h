//
// Created by uoyro on 5/15/23.
//

#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_VOXEL_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_VOXEL_H_

#include <glad/gl.h>

#include <bitset>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <unordered_map>

namespace glfw_learn::renderer {

using VoxelFace = std::uint8_t;
using VoxelSize = std::uint32_t;

constexpr VoxelFace kVoxelFront = 1;
constexpr VoxelFace kVoxelBack = 1 << 1;
constexpr VoxelFace kVoxelTop = 1 << 2;
constexpr VoxelFace kVoxelBottom = 1 << 3;
constexpr VoxelFace kVoxelRight = 1 << 4;
constexpr VoxelFace kVoxelLeft = 1 << 5;
constexpr VoxelFace kVoxelAll = kVoxelFront | kVoxelBack | kVoxelTop |
                                kVoxelBottom | kVoxelRight | kVoxelLeft;

struct Color {
  glm::u8vec4 color;
};

struct Voxel {
  VoxelSize size{};
  VoxelFace faces{kVoxelAll};
};

void VoxelTransformSystem(entt::registry& registry);

/*
 * Система для отрисовки вокселей.
 */
class VoxelRenderingSystem {
 public:
  VoxelRenderingSystem();
  ~VoxelRenderingSystem();

  void operator()(entt::registry& registry);
 private:
  struct Vertex {
    glm::vec3 position;
  };

  struct Instance {
    glm::mat4 transform;
    glm::vec4 color;
  };

  inline static const std::vector<Vertex> kVertices = {
      Vertex{glm::vec3{-1.0f, -1.0f, 1.0f}},   // 0
      Vertex{glm::vec3{1.0f, -1.0f, 1.0f}},    // 1
      Vertex{glm::vec3{1.0f, 1.0f, 1.0f}},     // 2
      Vertex{glm::vec3{-1.0f, 1.0f, 1.0f}},    // 3
      Vertex{glm::vec3{1.0f, -1.0f, -1.0f}},   // 4
      Vertex{glm::vec3{-1.0f, -1.0f, -1.0f}},  // 5
      Vertex{glm::vec3{-1.0f, 1.0f, -1.0f}},   // 6
      Vertex{glm::vec3{1.0f, 1.0f, -1.0f}},    // 7
  };
  inline static const std::vector<GLuint> kIndexes = {
    0, 1, 2, 0, 2, 3, 
    0, 5, 4, 0, 4, 1, 
    0, 6, 5, 0, 3, 6, 
    7, 4, 5, 7, 5, 6, 
    7, 6, 3, 7, 3, 2, 
    7, 2, 1, 7, 1, 4
  };

  GLuint vertex_array_{}, array_buffers_[2]{}, index_buffer_{}, program_;
};

}  // namespace glfw_learn::renderer

#endif  // GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_VOXEL_H_
