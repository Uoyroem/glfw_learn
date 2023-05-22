


add_executable (glfw_learn main.cc glfw_learn/camera.cc glfw_learn/renderer/voxel.cc glfw_learn/core/engine.cc glfw_learn/transform.cc glfw_learn/core/resource_manager.cc)



set_property (TARGET glfw_learn PROPERTY CXX_STANDARD 20)

include_directories (${INCLUDE_DIRS})


target_link_libraries (glfw_learn glfw glad glm EnTT OpenGL::GL)