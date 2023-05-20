//
// Created by uoyro on 5/15/23.
//

#include "glfw_learn/renderer/voxel.h"

#include <glad/gl.h>

#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "glfw_learn/camera.h"
#include "glfw_learn/transform.h"
#include "glfw_learn/core/resource.h"

using namespace entt::literals;

namespace glfw_learn::renderer {
void VoxelTransformSystem(entt::registry &registry) {
  auto view = registry.view<Transform, Voxel>();

  for (auto [_, transform, voxel] : view.each()) {
    transform.scale = glm::vec3(1.0f) / static_cast<float>(1 << voxel.size);
  }
}

VoxelRenderingSystem::VoxelRenderingSystem(entt::registry &registry) : registry_{registry} {
  using namespace core;

  auto voxel_vert = registry_.create();
  registry_.emplace<Resource>(voxel_vert,
                             std::filesystem::path("shaders/voxel.vert"));
  auto &voxel_vert_shader =
      registry_.emplace<Shader>(voxel_vert, GLenum(GL_VERTEX_SHADER));
  registry_.emplace<LoadResourceTag>(voxel_vert);
  registry_.emplace<CompileShaderTag>(voxel_vert);

  auto voxel_frag = registry_.create();
  registry_.emplace<Resource>(voxel_frag,
                             std::filesystem::path("shaders/voxel.frag"));
  auto &voxel_frag_shader =
      registry_.emplace<Shader>(voxel_frag, GLenum(GL_FRAGMENT_SHADER));
  registry_.emplace<LoadResourceTag>(voxel_frag);
  registry_.emplace<CompileShaderTag>(voxel_frag);

  auto voxel_program = registry_.create();
  program_ = &registry_.emplace<Program>(
      voxel_program, std::vector<Shader*>{&voxel_frag_shader, &voxel_vert_shader});
  registry.emplace<LinkProgramTag>(voxel_program);

  glGenBuffers(1, &vertex_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glBufferData(GL_ARRAY_BUFFER, kVertices.size() * sizeof(Vertex),
               kVertices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vertex_array_);
  glBindVertexArray(vertex_array_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_array_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        reinterpret_cast<void *>(offsetof(Vertex, position)));
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void VoxelRenderingSystem::operator()() {
  auto voxels = registry_.view<Voxel, Transform, Color>();

  auto program = program_->program;

  glUseProgram(program);
  glBindVertexArray(vertex_array_);

  GLint mvp_uniform_location = glGetUniformLocation(program, "mvp");
  GLint color_uniform_location = glGetUniformLocation(program, "color");

  auto [camera, camera_transform] = registry_.get<Camera, Transform>(
      registry_.view<Camera, Transform>().back());

  for (auto [_, voxel, voxel_transform, voxel_color] : voxels.each()) {
    if (!voxel.faces) {
      continue;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetIndexBuffer(voxel.faces));

    auto mvp = camera.projection * camera_transform.transform *
               voxel_transform.transform;
    auto color = glm::vec4(voxel_color.color) / 255.0f;

    glUniformMatrix4fv(mvp_uniform_location, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform4fv(color_uniform_location, 1, glm::value_ptr(color));
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(std::bitset<6>(voxel.faces).count()) * 6,
        GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  glBindVertexArray(0);
  glUseProgram(0);
}

VoxelRenderingSystem::~VoxelRenderingSystem() {
  glDeleteBuffers(1, &vertex_buffer_);
  for (auto [_, index_buffer] : index_buffers_) {
    glDeleteBuffers(1, &index_buffer);
  }
  glDeleteVertexArrays(1, &vertex_array_);
  glDeleteProgram(program_->program);
}
GLuint VoxelRenderingSystem::GetIndexBuffer(VoxelFace faces) {
  if (index_buffers_.contains(faces)) {
    return index_buffers_[faces];
  }

  GLuint index_buffer;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

  std::vector<GLuint> voxel_indexes;

  for (auto &[face, indexes] : kVoxelIndexesWithFaces) {
    if (faces & face) {
      voxel_indexes.insert(voxel_indexes.end(), indexes.begin(), indexes.end());
    }
  }

  index_buffers_[faces] = index_buffer;

  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(voxel_indexes.size() * sizeof(GLuint)),
               voxel_indexes.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return index_buffer;
}
}  // namespace glfw_learn::renderer
