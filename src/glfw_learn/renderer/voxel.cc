//
// Created by uoyro on 5/15/23.
//

#include "glfw_learn/renderer/voxel.h"

#include <memory>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "glfw_learn/camera.h"
#include "glfw_learn/transform.h"
#include "glfw_learn/core/engine.h"
#include "glfw_learn/core/resource_manager.h"

using namespace entt::literals;

namespace glfw_learn::renderer {
void VoxelTransformSystem(entt::registry &registry) {
  auto view = registry.view<Transform, Voxel>();

  for (auto [_, transform, voxel] : view.each()) {
    transform.scale = glm::vec3(1.0f) / static_cast<float>(1 << voxel.size);
  }
}

VoxelRenderingSystem::VoxelRenderingSystem() {
  using namespace core;
  auto engine = Engine::Get();

  auto resource_manager = engine->GetResourceManager();

  GLuint vert = glCreateShader(GL_VERTEX_SHADER);
  auto bytes = resource_manager->Load("shaders/voxel.vert");
  std::string vert_src{std::cbegin(bytes), std::cend(bytes)};
  const GLchar *vert_src_as_c_str = vert_src.c_str();
  glShaderSource(vert, 1, &vert_src_as_c_str, nullptr);
  glCompileShader(vert);

  GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
  bytes = resource_manager->Load("shaders/voxel.frag");
  std::string frag_src{std::cbegin(bytes), std::cend(bytes)};
  const GLchar *frag_src_as_c_str = frag_src.c_str();
  glShaderSource(frag, 1, &frag_src_as_c_str, nullptr);
  glCompileShader(frag);

  program_ = glCreateProgram();
  glAttachShader(program_, vert);
  glAttachShader(program_, frag);
  glLinkProgram(program_);
  glDetachShader(program_, vert);
  glDetachShader(program_, frag);

  glDeleteShader(vert);
  glDeleteShader(frag);

  glGenBuffers(2, array_buffers_);

  glBindBuffer(GL_ARRAY_BUFFER, array_buffers_[0]);
  glBufferData(GL_ARRAY_BUFFER, kVertices.size() * sizeof(Vertex),
               kVertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, array_buffers_[1]);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(1024 * 1024 * 8),
               nullptr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &index_buffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, kIndexes.size() * sizeof(GLuint),
               kIndexes.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vertex_array_);
  glBindVertexArray(vertex_array_);

  glBindBuffer(GL_ARRAY_BUFFER, array_buffers_[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                        reinterpret_cast<void *>(offsetof(Vertex, position)));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, array_buffers_[1]);

  size_t offset = offsetof(Instance, transform);
  size_t intance_size = sizeof(Instance);
  size_t vec4_size = sizeof(glm::vec4);

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, intance_size,
                        reinterpret_cast<void *>(offset));
  glVertexAttribDivisor(1, 1);
  glEnableVertexAttribArray(1);

  offset += vec4_size;
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, intance_size,
                        reinterpret_cast<void *>(offset));
  glVertexAttribDivisor(2, 1);
  glEnableVertexAttribArray(2);

  offset += vec4_size;
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, intance_size,
                        reinterpret_cast<void *>(offset));
  glVertexAttribDivisor(3, 1);
  glEnableVertexAttribArray(3);

  offset += vec4_size;
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, intance_size,
                        reinterpret_cast<void *>(offset));
  glVertexAttribDivisor(4, 1);
  glEnableVertexAttribArray(4);

  offset = offsetof(Instance, color);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, intance_size,
                        reinterpret_cast<void *>(offset));
  glVertexAttribDivisor(5, 1);
  glEnableVertexAttribArray(5);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
  glBindVertexArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VoxelRenderingSystem::~VoxelRenderingSystem() {
  glDeleteBuffers(2, array_buffers_);
  glDeleteBuffers(1, &index_buffer_);
  glDeleteVertexArrays(1, &vertex_array_);
  glDeleteProgram(program_);
}

void VoxelRenderingSystem::operator()(entt::registry& registry) {
  auto voxels = registry.view<Voxel, Transform, Color>();
  
  auto program = program_;

  glUseProgram(program);
  glBindVertexArray(vertex_array_);

  std::vector<Instance> instances;
  for (auto [entity, voxel, transform, color] : voxels.each()) {
    instances.emplace_back(transform.transform,
                           glm::vec4(color.color) / 255.0f);
  }
  
  glBindBuffer(GL_ARRAY_BUFFER, array_buffers_[1]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(Instance),
                  instances.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  auto [camera, transform] = registry.get<Camera, Transform>(
      registry.view<Camera, Transform>().back());

  GLint vp_uniform_location = glGetUniformLocation(program, "vp");
  auto vp = camera.projection * transform.transform;
  glUniformMatrix4fv(vp_uniform_location, 1, GL_FALSE, glm::value_ptr(vp));

  glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr,
                          instances.size());

  glBindVertexArray(0);
  glUseProgram(0);
}
}  // namespace glfw_learn::renderer
