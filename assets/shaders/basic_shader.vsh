#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 color;

out vec4 v_color;
out vec2 v_uv;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj;

void main()
{
  gl_Position = proj * view * model * vec4(position, 1.0);
  v_color = color;
  v_uv = vec2(uv.x, 1.0 - uv.y);
}

