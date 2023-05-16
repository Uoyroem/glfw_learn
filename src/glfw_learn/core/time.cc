//
// Created by uoyro on 5/14/23.
//

#include "glfw_learn/core/time.h"

namespace glfw_learn::core {
extern float delta_time;

float GetDeltaTime() {
  return delta_time;
}
}
