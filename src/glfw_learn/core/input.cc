
#include "glfw_learn/core/input.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <map>

namespace glfw_learn::core {
static std::map<int, bool> keys;
static float mouse_x = 0, mouse_y = 0;

bool IsKeyPressed(int key) {
  return keys.contains(key) && keys.at(key);
}
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  switch (action) {
    case GLFW_PRESS:keys[key] = true;
      break;
    case GLFW_RELEASE:keys[key] = false;
      break;
    default:break;
  }
}
void CursorPosCallback(GLFWwindow *window, double x, double y) {
  mouse_x = static_cast<float>(x);
  mouse_y = static_cast<float>(y);
}
float GetMouseX() { return mouse_x; }
float GetMouseY() { return mouse_y; }
}
