
#include "glfw_learn/core/engine.h"

int main(int argc, char **argv) {
  using namespace glfw_learn::core;

  Engine::Initialize(argv[0]);
  auto engine = Engine::Get();
  engine->Run();

  return 0;
}
