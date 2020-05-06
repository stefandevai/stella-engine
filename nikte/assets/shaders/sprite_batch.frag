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
  vec4 final_color = f_in.color;
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
  
  bool lighting = true;
  if (lighting)
  {
  vec2 screen_resolution = vec2(896.0, 504.0);
  bool morning = true;

  if (morning)
  {
    vec3 morning_color_shift = vec3(1.9, 0.9, 0.8);
    float vignette_intensity = 0.8;
    float vignette_spread = 2.0;
    float vignette_radius = 600.0*vignette_spread;
    vec2 vignette_center = vec2(screen_resolution.x/2.0, screen_resolution.y/2.0);
    float dist_center = distance(vignette_center, gl_FragCoord.xy);

    float vignette_factor = qinticIn(smoothstep(vignette_radius, 0, dist_center))*vignette_intensity;
    final_color.rg *= morning_color_shift.rg*(vignette_factor+0.1);
    final_color.b *= 0.4;
  }
  else
  {
    vec3 night_color_shift = vec3(0.3, 0.7, 1.8);
    float vignette_intensity = 0.7;
    float vignette_spread = 2.0;
    float vignette_radius = 500.0*vignette_spread;
    vec2 vignette_center = vec2(screen_resolution.x/2.0, screen_resolution.y/2.0);
    float dist_center = distance(vignette_center, gl_FragCoord.xy);

    float vignette_factor = qinticIn(smoothstep(vignette_radius, 0, dist_center))*vignette_intensity;
  
    final_color.rgb *= night_color_shift*vignette_factor;
  }
  }
  
  if (final_color.a * f_in.color.a < 0.5)
  {
    discard;
  }
  vec3 fog_factor = vec3(smoothstep(0, 2, gl_FragCoord.z) - 0.1);
  color = vec4(final_color.rgb + fog_factor*4.0, final_color.a * f_in.color.a);
}








