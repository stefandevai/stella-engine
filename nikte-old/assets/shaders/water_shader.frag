#version 330 core

in DATA
{
  vec4 color;
  vec2 pos;
  vec3 barymetric;
  flat float is_top;
  flat vec3 shape_position;
  flat vec2 shape_dimensions;
} f_in;

out vec4 color;

void main()
{
  float globaly = f_in.shape_position.y;
  float height = globaly + f_in.shape_dimensions.y;
  float posy = f_in.pos.y;
  float highlight_edge_height = 0.03;
  vec4 final_color = f_in.color;
  final_color.g *= (2.7 - 2.0*posy/height);
  final_color.rb *= (1.2 - 0.7*posy/height);

  bvec3 edge_comparison = lessThan(f_in.barymetric, vec3(1.0 - highlight_edge_height, highlight_edge_height, highlight_edge_height));
  if (edge_comparison.z && f_in.is_top > 0.5)
  {
    final_color.g *= 1.8;
    final_color.b *= 1.2;
    final_color.r *= 1.1;
  }
  else if (edge_comparison.z && !edge_comparison.x)
  {
    final_color.g *= 1.8;
    final_color.b *= 1.2;
    final_color.r *= 1.1;
  }

  color = vec4(final_color);
}

