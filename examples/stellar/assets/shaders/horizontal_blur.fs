#version 330 core

out vec4 FragColor;
  
in vec2 TexCoords;
uniform sampler2D ScreenTex;

uniform bool horizontal;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{ 
	vec2 offset = 1.0 / textureSize(ScreenTex, 0);
	vec3 result = texture(ScreenTex, TexCoords).rgb * weight[0];

	if (horizontal) {
		for (int i = 1; i < 5; ++i) {
			result += texture(ScreenTex, TexCoords + vec2(offset.x * i, 0.0)).rgb * weight[i];
			result += texture(ScreenTex, TexCoords - vec2(offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else {
		for (int i = 1; i < 5; ++i) {
			result += texture(ScreenTex, TexCoords + vec2(0.0, offset.y * i)).rgb * weight[i];
			result += texture(ScreenTex, TexCoords - vec2(0.0, offset.y * i)).rgb * weight[i];
		}
	}

	FragColor = vec4(result, 1.0);
}

