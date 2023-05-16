
#include "glfw_learn/core/input.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <map>

namespace glfw_learn::core {
std::map<int, bool> keys;
float mouse_ox = 0, mouse_oy = 0;

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
  static bool initialized = false;
  static double mouse_x, mouse_y;

  if (initialized) {
    mouse_ox = static_cast<float>(mouse_x - x);
    mouse_oy = static_cast<float>(mouse_y - y);
  }

  mouse_x = x;
  mouse_y = y;

  if (!initialized) {
    initialized = true;
  }
}
float &GetXMouseOffset() {
  return mouse_ox;
}
float &GetYMouseOffset() {
  return mouse_oy;
}
}
