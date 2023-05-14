
#include <iostream>
#include <string_view>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace entt::literals;

constexpr std::string_view title = "GLFW Learn";


GLFWmonitor* monitor = nullptr;
const GLFWvidmode* vidmode = nullptr;
GLFWwindow* window = nullptr;
float aspect;

// Компонент Voxel - указывает что сущность может быть отрендеровано как воксель.
struct Voxel {
  std::uint32_t division;
  glm::ivec3 position;
};

struct Transform {};

void GLFWErrorCallback(int error_code, const char* description);
void Initialize();
void SetWindowHints();

GLuint vao, vbo, ebo, shader_program;

constexpr const GLchar* vertex_shader_source = R"(
#version 420 core

layout (location = 0) in vec3 in_position;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * transform * vec4(in_position, 1.0f);
}
)";

constexpr const GLchar* fragment_shader_source = R"(
#version 420 core

out vec4 out_color;

void main() {
  out_color = vec4(1.0f);
}
)";

void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                         GLsizei length, const GLchar* message,
                         const void* user_param);
void InitializeOpenGL();
void Render(entt::registry&);
