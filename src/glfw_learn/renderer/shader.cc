
#include "glfw_learn/renderer/shader.h"
#include "glfw_learn/core/resource.h"

#include <glad/gl.h>

namespace glfw_learn::renderer {
void ShaderCompilingSystem(entt::registry& registry) {
  using namespace core;

  auto view = registry.view<Shader, CompileShaderTag, Resource>();

  for (auto [entity, shader, resource] : view.each()) {
    shader.shader = glCreateShader(shader.type);
    auto src_string = std::string{resource.bytes.begin(), resource.bytes.end()};
    const GLchar* src = src_string.c_str();
    glShaderSource(shader.shader, 1, &src, nullptr);
    glCompileShader(shader.shader);

    registry.remove<CompileShaderTag>(entity);
  }
}
void ProgramLinkingSystem(entt::registry& registry) {
  auto view = registry.view<Program, LinkProgramTag>();

  for (auto [entity, program] : view.each()) {
    program.program = glCreateProgram();
    for (auto shader : program.shaders) {
      glAttachShader(program.program, shader->shader);
    }
    glLinkProgram(program.program);
    for (auto shader : program.shaders) {
      glDetachShader(program.program, shader->shader);
    }

    registry.remove<LinkProgramTag>(entity);
  }
}
}  // namespace glfw_learn::renderer