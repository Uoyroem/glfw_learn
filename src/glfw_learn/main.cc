
#include "glfw_learn/main.h"


int main(int argc, char** argv) {
  Initialize();


  entt::registry registry;
  auto voxel1 = registry.create();
  
  registry.ctx().emplace_as<glm::mat4>(
      "view"_hs, glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -5.0f)));
  registry.ctx().emplace_as<glm::mat4>(
      "projection"_hs, glm::perspective(glm::radians(60.0f), aspect, 0.001f, 1000.0f));

  registry.emplace<Voxel>(voxel1, 4U, glm::ivec4(0));

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Render(registry);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
	return 0;
}

void GLFWErrorCallback(int error_code, const char* description) {
  std::cout << description << std::endl;
}

void Initialize() {
  glfwSetErrorCallback(GLFWErrorCallback);

  glfwInit();
  monitor = glfwGetPrimaryMonitor();
  vidmode = glfwGetVideoMode(monitor);

  aspect =
      static_cast<float>(vidmode->width) / static_cast<float>(vidmode->height);

  SetWindowHints();
  
  window = glfwCreateWindow(vidmode->width, vidmode->height, title.data(),
                            monitor, nullptr);
  glfwMakeContextCurrent(window);

  InitializeOpenGL();
}

void SetWindowHints() {
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
}

void InitializeOpenGL() {
  gladLoadGL(glfwGetProcAddress);

  glDebugMessageCallback(OpenGLDebugCallback, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                        GL_TRUE);

  glViewport(0, 0, vidmode->width, vidmode->height);

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertex_shader, 1, &vertex_shader_source,
                 0);
  glCompileShader(vertex_shader);

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, 0);
  glCompileShader(fragment_shader);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glDetachShader(shader_program, vertex_shader);
  glDetachShader(shader_program, fragment_shader);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  const GLfloat vertices[] = {
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
  };
  const GLuint indeсes[] = {
    0, 1, 3, 0, 3, 2,
    0, 2, 6, 0, 6, 4,
    0, 1, 5, 0, 5, 4,
    7, 5, 1, 7, 1, 3,
    7, 6, 2, 7, 2, 3,
    7, 6, 4, 7, 4, 5
  };

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeсes), indeсes,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

void Render(entt::registry& registry) {
  glUseProgram(shader_program);

  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  GLuint transform_uniform_location =
      glGetUniformLocation(shader_program, "transform");
  GLuint view_uniform_location = glGetUniformLocation(shader_program, "view");
  GLuint projection_uniform_location = glGetUniformLocation(shader_program, "projection");
  

  for (auto [entity, voxel] : registry.view<Voxel>().each()) {
    glm::mat4 transform;
    transform = glm::translate(transform, glm::vec3(voxel.position));
    glUniformMatrix4fv(transform_uniform_location, 1, GL_FALSE,
                       glm::value_ptr(transform));

    glm::mat4& view = registry.ctx().get<glm::mat4>("view"_hs);

    glUniformMatrix4fv(
        view_uniform_location, 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(
        projection_uniform_location, 1, GL_FALSE,
        glm::value_ptr(registry.ctx().get<glm::mat4>("projection"_hs)));
    

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
  

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glUseProgram(0);
}

void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                         GLsizei length, const GLchar* message,
                         const void* user_param) {
  std::cout << message << std::endl;
}
