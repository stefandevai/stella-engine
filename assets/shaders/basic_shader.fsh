#version 330 core

in vec4 v_color;
in vec2 v_uv;

out vec4 color;

uniform sampler2D tex;

void main()
{
  color = texture(tex, v_uv) * v_color;
}

