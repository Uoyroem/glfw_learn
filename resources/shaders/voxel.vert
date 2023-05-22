#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in mat4 model;
layout (location = 5) in vec4 color;
uniform mat4 vp;

out vec4 ocolor;

void main() {
  ocolor = color;
  gl_Position = vp * model * vec4(position, 1.0f);
}