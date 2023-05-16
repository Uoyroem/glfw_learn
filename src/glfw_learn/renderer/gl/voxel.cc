//
// Created by uoyro on 5/15/23.
//

#include "glfw_learn/renderer/gl/voxel.h"
#include "glfw_learn/camera.h"
#include "glfw_learn/transform.h"
#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"
#include "entt/entt.hpp"

using namespace entt::literals;

namespace glfw_learn::renderer::gl {
constexpr const GLchar *voxel_vert = R"(
#version 420 core

layout (location = 0) in vec3 position;
uniform mat4 mvp;

void main() {
  gl_Position = mvp * vec4(position, 1.0f);
}
)";
constexpr const GLchar *voxel_frag = R"(
#version 420 core

uniform vec4 color;
out vec4 result;

void main() {
  result = color;
}
)";
const GLfloat voxel_vertices[] = {
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
};
const GLuint voxel_indexes[] = {
    0, 1, 3, 0, 3, 2,
    0, 2, 6, 0, 6, 4,
    0, 1, 5, 0, 5, 4,
    7, 5, 1, 7, 1, 3,
    7, 6, 2, 7, 2, 3,
    7, 6, 4, 7, 4, 5
};

VoxelSystem::VoxelSystem() {
  GLuint vert = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vert, 1, &voxel_vert,
                 nullptr);
  glCompileShader(vert);

  GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &voxel_frag, nullptr);
  glCompileShader(frag);

  program_ = glCreateProgram();
  glAttachShader(program_, vert);
  glAttachShader(program_, frag);
  glLinkProgram(program_);

  glDetachShader(program_, vert);
  glDetachShader(program_, frag);

  glDeleteShader(vert);
  glDeleteShader(frag);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(voxel_vertices), voxel_vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(voxel_indexes), voxel_indexes,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vao_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBindVertexArray(0);
}
void VoxelSystem::operator()(entt::registry &registry) {
  auto voxels = registry.view<Voxel, Transform, Color>();

  glUseProgram(program_);
  glBindVertexArray(vao_);

  GLint mvp_uniform_location = glGetUniformLocation(program_, "mvp");
  GLint color_uniform_location = glGetUniformLocation(program_, "color");

  auto [camera, camera_transform] = registry.get<Camera, Transform>(registry.view<Camera, Transform>().back());

  for (auto [_, voxel, voxel_transform, voxel_color] : voxels.each()) {
    auto mvp = camera.projection * camera_transform.transform * voxel_transform.transform;
    auto color = glm::vec4(voxel_color.color) / 255.0f;

    glUniformMatrix4fv(mvp_uniform_location,
                       1,
                       GL_FALSE,
                       glm::value_ptr(mvp));
    glUniform4fv(color_uniform_location, 1, glm::value_ptr(color));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
  }

  glBindVertexArray(0);
  glUseProgram(0);
}
VoxelSystem::~VoxelSystem() {
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ibo_);
  glDeleteVertexArrays(1, &vao_);
  glDeleteProgram(program_);
}
}
