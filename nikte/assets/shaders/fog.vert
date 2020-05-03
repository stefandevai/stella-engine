#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in float tid;
layout (location = 3) in float yorigin;
layout (location = 4) in float height;
layout (location = 5) in int is_flat;

out DATA
{
  vec2 uv;
  float tid;
  vec3 pos;
  float yorigin;
  float height;
  float is_flat;
} v_out;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj;

void main()
{
  vec4 final_pos = proj * view * model * vec4(position, 1.0);
  gl_Position = final_pos;
  v_out.uv = vec2(uv.x, 1.0 - uv.y);
  v_out.tid = tid;
  v_out.pos = position.xyz;
  v_out.yorigin = yorigin;
  v_out.height = height;
  v_out.is_flat = is_flat;
}

// #version 330 core

// layout (location = 0) in vec3 position;
// layout (location = 1) in vec4 color;

// out DATA
// {
//   vec2 pos;
//   vec4 color;
// } v_out;

// uniform mat4 model = mat4(1.0);
// uniform mat4 view = mat4(1.0);
// uniform mat4 proj;

// void main()
// {
//   gl_Position = proj * view * model * vec4(position, 1.0);
//   v_out.color = color;
//   v_out.pos = vec2(position.xy);
// }
