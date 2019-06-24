#version 330 core

out vec4 FragColor;
  
in vec2 TexCoords;
uniform sampler2D ScreenTex;

uniform bool horizontal;
const int sizek = 7;
//uniform float weight[sizek] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
//uniform float weight[sizek] = float[](0.198596,0.175713,0.121703,0.065984,0.028002,0.0093);
uniform float weight[sizek] = float[](0.136498,0.129188,0.109523,0.083173,0.056577,0.034474,0.018816);

void main()
{ 
	vec2 offset = 1.0 / textureSize(ScreenTex, 0);
	vec3 result = texture(ScreenTex, TexCoords).rgb * weight[0];
	FragColor = texture(ScreenTex, TexCoords);

	if (horizontal) {
		for (int i = 1; i < sizek; ++i) {
			result += texture(ScreenTex, TexCoords + vec2(offset.x * i, 0.0)).rgb * weight[i];
			result += texture(ScreenTex, TexCoords - vec2(offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else {
		for (int i = 1; i < sizek; ++i) {
			result += texture(ScreenTex, TexCoords + vec2(0.0, offset.y * i)).rgb * weight[i];
			result += texture(ScreenTex, TexCoords - vec2(0.0, offset.y * i)).rgb * weight[i];
		}
	}

	FragColor = vec4(result, FragColor.a);
}

