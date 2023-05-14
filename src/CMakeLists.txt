# CMakeList.txt: проект CMake для glfw_learn; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (glfw_learn "glfw_learn/main.cc")

set_property (TARGET glfw_learn PROPERTY CXX_STANDARD 20)

include_directories (${INCLUDE_DIR})
# TODO: Добавьте тесты и целевые объекты, если это необходимо.

target_link_libraries (glfw_learn glfw glad glm EnTT OpenGL::GL)