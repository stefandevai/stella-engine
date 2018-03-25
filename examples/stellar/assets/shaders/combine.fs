#version 330 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D NormalScene;
uniform sampler2D PostScene;

uniform float intensity = 9.0;

void main()
{ 
		vec4 NormalColor = texture(NormalScene, TexCoords);
		vec4 PostColor = texture(PostScene, TexCoords);
		
		FragColor = NormalColor * PostColor*intensity + PostColor*1.5;
}


