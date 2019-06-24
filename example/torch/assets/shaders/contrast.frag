#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float contrast = 0.5;

void main()
{ 
		// Contrast
		FragColor = texture(screenTexture, TexCoords);
		FragColor = vec4((FragColor.rgb - 0.5) * (1.0 + contrast) + 0.5, FragColor.a);
}

