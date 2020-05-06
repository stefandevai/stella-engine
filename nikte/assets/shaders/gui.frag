#version 330 core

in DATA
{
  vec2 uv;
  float tid;
  vec4 color;
  vec2 pos;
} f_in;

out vec4 color;

uniform sampler2D textures[11];

float qinticIn(float t) {
  return pow(t, 5.0);
}

void main()
{
  vec4 final_color = vec4(0.f);
  int tid = int(f_in.tid + 0.5);

  // Select texture (Up to 20 textures)
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
  }
  
  if (final_color.a * f_in.color.a < 0.5)
  {
    discard;
  }
  color = vec4(final_color.rgb, final_color.a * f_in.color.a);
}