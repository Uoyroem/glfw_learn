
#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_RESOURCE_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_RESOURCE_H_
#include <filesystem>
#include <vector>

#include <entt/entt.hpp>

namespace glfw_learn::core {
struct Resource {
  std::filesystem::path relative_path;
  std::vector<char> bytes{};
};

struct LoadResourceTag {};

class ResourceSystem {
 public:
  inline ResourceSystem(const std::filesystem::path& resources_path)
      : resources_path_{resources_path} {}

  void operator()(entt::registry& registry);

 private:
  std::filesystem::path resources_path_;
};

void LoadResourceSystem(entt::registry& registry);
}  // namespace glfw_learn::core

#endif  // !GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_RESOURCE_H_
