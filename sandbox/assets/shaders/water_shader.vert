#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 barymetric;
layout (location = 3) in uint is_top;

out DATA
{
  vec4 color;
  vec2 pos;
  vec3 barymetric;
  flat float is_top;
} v_out;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj;

void main()
{
  gl_Position = proj * view * model * vec4(position, 1.0);
  v_out.color = color;
  v_out.pos = vec2(position.xy);
  v_out.barymetric = barymetric;

  if (bool(is_top))
  {
    v_out.is_top = 1.0;
  }
  else
  {
    v_out.is_top = 0.0;
  }
}

