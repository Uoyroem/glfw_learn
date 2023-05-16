//
// Created by uoyro on 5/15/23.
//

#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_WINDOW_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_WINDOW_H_

struct GLFWwindow;

namespace glfw_learn::core {
void Run();
float GetAspect();
int GetWidth();
int GetHeight();
}

#endif //GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_WINDOW_H_
