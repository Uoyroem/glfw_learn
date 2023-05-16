//
// Created by uoyro on 5/15/23.
//

#ifndef GLFW_LEARN_SRC_GLFW_LEARN_RENDERER_GL_H_
#define GLFW_LEARN_SRC_GLFW_LEARN_RENDERER_GL_H_

#include <cstdint>

namespace glfw_learn::renderer::gl {
void InitializeOpenGL();
void OpenGLDebugCallback(uint32_t source,
                         uint32_t type,
                         uint32_t id,
                         uint32_t severity,
                         int32_t length,
                         const char *message,
                         const void *user_param);
}

#endif //GLFW_LEARN_SRC_GLFW_LEARN_RENDERER_GL_H_
