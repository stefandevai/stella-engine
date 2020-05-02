#version 330 core
out vec4 FragColor;
  
in DATA
{
  // vec2 uv;
  // float tid;
  
  vec2 pos;
  vec4 color;
} v_in;

void main()
{ 
	FragColor = v_in.color;
}