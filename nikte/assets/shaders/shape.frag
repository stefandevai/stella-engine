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
  color = final_color;
  //color = vec4(vec3(smoothstep(0, 2, gl_FragCoord.z) - 0.1), 1.0);
}

