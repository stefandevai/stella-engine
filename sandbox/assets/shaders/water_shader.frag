#version 330 core

in DATA
{
  vec4 color;
  vec2 pos;
} f_in;

out vec4 color;

void main()
{
  vec4 final_color = f_in.color;
  color = vec4(final_color.xyz, 0.5);
}

