#version 330 core
out vec4 FragColor;
  
in DATA
{
  // vec2 uv;
  // float tid;
  // vec4 color;
  vec2 pos;
} v_in;

void main()
{ 
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}