#version 330 core

out vec4 FragColor;
 
in vec2 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_normal1;
uniform sampler2D texture_normal2;
uniform sampler2D texture_normal3;

void main()
{ 
  FragColor = texture(texture_diffuse1, TexCoord);
  //FragColor = vec4(0.9, 0.6, 0.3, 1.0);
}


