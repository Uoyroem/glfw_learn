#include "glfw_learn/core/resource.h"

#include <fstream>
#include <algorithm>

namespace glfw_learn::core {
void ResourceSystem::operator()(entt::registry& registry) {
  auto view = registry.view<Resource, LoadResourceTag>();

  for (auto [entity, resource] : view.each()) {
    std::ifstream resource_file{resources_path_ / resource.relative_path,
                                std::ios_base::binary};
    resource.bytes =
        std::vector<char>(std::istreambuf_iterator<char>(resource_file), {});
    registry.remove<LoadResourceTag>(entity);
  }
}
}