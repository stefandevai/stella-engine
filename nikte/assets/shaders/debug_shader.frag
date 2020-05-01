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

  //if (final_color.a < 0.1)
  //{
    //discard;
  //}
  //color = vec4(final_color.xyz, 0.5);
  color = vec4(0.9, 0.6, 0.3, 1.0);
}

