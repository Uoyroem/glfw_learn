
add_executable(glfw_learn main.cc glfw_learn/camera.cc glfw_learn/core/input.cc glfw_learn/core/time.cc glfw_learn/renderer/gl/voxel.cc glfw_learn/renderer/gl/gl.cc glfw_learn/core/window.cc glfw_learn/transform.cc)

set_property(TARGET glfw_learn PROPERTY CXX_STANDARD 20)

include_directories(${INCLUDE_DIR})


target_link_libraries(glfw_learn glfw glad glm EnTT OpenGL::GL)