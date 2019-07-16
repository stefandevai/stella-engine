#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 barymetric;
layout (location = 3) in vec3 shape_position;
layout (location = 4) in vec2 shape_dimensions;

out DATA
{
  vec4 color;
  vec2 pos;
  vec3 barymetric;
  flat float is_top;
  flat vec3 shape_position;
  flat vec2 shape_dimensions;
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
  v_out.shape_position = shape_position;
  v_out.shape_dimensions = shape_dimensions;

  if (gl_VertexID % 2 == 0)
  {
    v_out.is_top = 1.0;
  }
  else
  {
    v_out.is_top = 0.0;
  }
}

