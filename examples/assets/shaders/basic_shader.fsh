#version 330 core

in DATA
{
  vec2 uv;
  float tid;
  vec4 color;
  vec2 pos;
} f_in;

out vec4 color;

uniform sampler2D textures[32];
uniform vec2 lightPos;
uniform float lightCounter;
uniform float lightIntensity;

void main()
{
  float light = (10*cos(lightCounter/5.0) + 100.0)/length(f_in.pos - lightPos) * lightIntensity;
  if (light > 1.5) light = 1.5;
  else if (light < 0.5) light = 0.5;

  vec4 final_color = f_in.color;
  int tid = int(f_in.tid + 0.5);

  // Select texture (Up to 12 textures)
  switch (tid)
  {
    case 0:
      final_color = texture(textures[0], f_in.uv);
      break;
    case 1:
      final_color = texture(textures[1], f_in.uv);
      break;
    case 2:
      final_color = texture(textures[2], f_in.uv);
      break;
    case 3:
      final_color = texture(textures[3], f_in.uv);
      break;
    case 4:
      final_color = texture(textures[4], f_in.uv);
      break;
    case 5:
      final_color = texture(textures[5], f_in.uv);
      break;
    case 6:
      final_color = texture(textures[6], f_in.uv);
      break;
    case 7:
      final_color = texture(textures[7], f_in.uv);
      break;
    case 8:
      final_color = texture(textures[8], f_in.uv);
      break;
    case 9:
      final_color = texture(textures[9], f_in.uv);
      break;
    case 10:
      final_color = texture(textures[10], f_in.uv);
      break;
    case 11:
      final_color = texture(textures[11], f_in.uv);
      break;
  } 

  color = vec4(final_color.xyz * f_in.color.xyz, final_color.w * f_in.color.w);
}

