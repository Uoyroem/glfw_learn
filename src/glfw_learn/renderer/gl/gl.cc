//
// Created by uoyro on 5/15/23.
//

#include "glfw_learn/renderer/gl/gl.h"

#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace glfw_learn::renderer::gl {
void InitializeOpenGL() {
  gladLoadGL(glfwGetProcAddress);
  glDebugMessageCallback(OpenGLDebugCallback, nullptr);
}
void OpenGLDebugCallback(GLenum source,
                         GLenum type,
                         GLuint id,
                         GLenum severity,
                         GLsizei length,
                         const GLchar *message,
                         const void *user_param) {
  std::cout << message << std::endl;
}
}
