
#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_RESOURCE_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_RESOURCE_H_

#include <filesystem>
#include <unordered_map>
#include <vector>

namespace glfw_learn::core {
class ResourceManager final {
 public:
  inline ResourceManager(const std::filesystem::path& resources_path) noexcept
      : resources_path_{resources_path} {}

  std::vector<char> Load(
      const std::filesystem::path& relative_path) const noexcept;

 private:
  std::filesystem::path resources_path_;
};
}  // namespace glfw_learn::core

#endif  // !GLFW_LEARN_INCLUDE_GLFW_LEARN_CORE_RESOURCE_H_
