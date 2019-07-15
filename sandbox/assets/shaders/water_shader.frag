#version 330 core

in DATA
{
  vec4 color;
  vec2 pos;
  vec3 barymetric;
  flat float is_top;
} f_in;

out vec4 color;

void main()
{
  float globaly = 200.0;
  float height = globaly + 128.0;
  float posy = f_in.pos.y;
  vec4 final_color = f_in.color;
  final_color.g *= (2.7 - 2.0*posy/height);
  final_color.rb *= (1.2 - 0.7*posy/height);

  bvec3 edge_comparisonl = lessThan(f_in.barymetric, vec3(0.08, 0.08, 0.08));
  bvec3 edge_comparisong = greaterThan(f_in.barymetric, vec3(0.92, 0.08, 0.08));
  if (edge_comparisonl.z && f_in.is_top > 0.5)
  {
    final_color.g *= 3.0;
    final_color.ba *= 1.2;
    final_color.r *= 1.1;
  }
  else if (edge_comparisonl.z && edge_comparisong.x)
  {
    final_color.g *= 3.0;
    final_color.ba *= 1.2;
    final_color.r *= 1.1;
  }

  color = vec4(final_color);
}

