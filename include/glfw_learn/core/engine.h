//
// Created by uoyro on 5/15/23.
//

#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_WINDOW_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_WINDOW_H_

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <memory>
#include <string_view>

#include "glfw_learn/core/resource_manager.h"
#include "glfw_learn/renderer/voxel.h"

namespace glfw_learn::core {
class Engine final {
 public:
  ~Engine() noexcept;

  static void Initialize(const std::string_view& argv0);
  inline static std::shared_ptr<Engine> Get() noexcept { return instance_; }

  void Run();

  inline GLFWwindow* GetWindow() const noexcept { return window_; }
  inline std::shared_ptr<ResourceManager> GetResourceManager() noexcept {
    return resource_manager_;
  }

  inline float GetDeltaTime() const noexcept { return delta_time_; }

  inline bool GetKey(int key) const noexcept {
    return glfwGetKey(window_, key) == GLFW_PRESS;
  }
  inline float GetMouseX() const noexcept {
    double x;
    glfwGetCursorPos(window_, &x, nullptr);
    return static_cast<float>(x);
  }
  inline float GetMouseY() const noexcept {
    double y;
    glfwGetCursorPos(window_, nullptr, &y);
    return static_cast<float>(y);
  }
  
 private:
  Engine(const std::string_view& argv0);
  

  GLFWwindow* window_;
  std::shared_ptr<ResourceManager> resource_manager_;

  float delta_time_;

  inline static std::shared_ptr<Engine> instance_{};

  static void OpenGLDebugCallback_(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length,
                                   const GLchar* message,
                                   const void* user_param);
};
}  // namespace glfw_learn::core

#endif  // GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_WINDOW_H_
