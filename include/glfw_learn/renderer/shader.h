
#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_SHADER_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_SHADER_H_

#include <glad/gl.h>

#include <entt/entt.hpp>

namespace glfw_learn::renderer {
struct Shader {
  GLenum type;
  GLuint shader{};
};

struct Program {
  std::vector<Shader*> shaders;
  GLuint program{};
};

struct CompileShaderTag {};
struct LinkProgramTag {};

void ShaderCompilingSystem(entt::registry& registry);
void ProgramLinkingSystem(entt::registry& registry);
}  // namespace glfw_learn::renderer

#endif  //  GLFW_LEARN_INCLUDE_GLFW_LEARN_RENDERER_SHADER_H_