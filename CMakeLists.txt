# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.24)

project ("glfw_learn")

set (INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
set (RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${glfw_learn_BINARY_DIR}/bin")

file (COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

find_package (OpenGL REQUIRED)

# Включите подпроекты.
add_subdirectory ("vendors/glfw")
add_subdirectory ("vendors/glad/cmake")
add_subdirectory ("vendors/glm")
add_subdirectory ("vendors/entt")
glad_add_library (glad API gl:core=4.2)
add_subdirectory ("src")
