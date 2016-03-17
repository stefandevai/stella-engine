#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in float tid;
layout (location = 3) in vec4 color;

out DATA
{
  vec2 uv;
  float tid;
  vec4 color;
} v_out;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj;

void main()
{
  gl_Position = proj * view * model * vec4(position, 1.0);
  v_out.uv = vec2(uv.x, 1.0 - uv.y);
  v_out.tid = tid;
  v_out.color = color;
}

