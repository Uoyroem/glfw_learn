#include "glfw_learn/core/resource_manager.h"

#include <algorithm>
#include <fstream>

namespace glfw_learn::core {
std::vector<char> ResourceManager::Load(
    const std::filesystem::path& relative_path) const noexcept {
  auto absolute_path = resources_path_ / relative_path;

  std::ifstream resource{absolute_path, std::ifstream::binary};

  std::vector<char> bytes;
  bytes.insert(std::begin(bytes), std::istreambuf_iterator<char>(resource),
               std::istreambuf_iterator<char>());

  return bytes;
}
}  // namespace glfw_learn::core