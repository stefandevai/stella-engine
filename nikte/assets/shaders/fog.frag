#version 330 core

in DATA
{
  vec2 uv;
  float tid;
  vec3 pos;
  float yorigin;
  float height;
} f_in;

out vec4 color;

uniform sampler2D textures[11];
void main()
{
  vec4 final_color = vec4(1.0, 1.0, 1.0, 1.0);
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
  
  float fog_height = 64.0;
  float fog_intensity = 0.5;
  float frag_height = f_in.height - f_in.pos.y;
  float fog_factor = (fog_height - frag_height) / fog_height;
  float final_fog = fog_factor*fog_intensity;
  
  // if (f_in.pos.y > 0.0 && f_in.pos.y < 10.0)
  // {
  //   final_color.w = 1.0;
  // }
  // else
  // {
  //   final_color.w = 0.0;
  // }
  //color = vec4(1.0, 1.0, 1.0, final_color.w*fog_factor);
  color = vec4(1.0, 1.0, 1.0, final_color.w*final_fog);
}

// #version 330 core
// out vec4 FragColor;
  
// in DATA
// {
//   vec2 pos;
//   vec4 color;
// } v_in;

// void main()
// { 
// 	FragColor = v_in.color;
// }