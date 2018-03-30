#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float contrast = 0.5;

void main()
{ 
		// Don't postprocess the scene
		//FragColor = texture(screenTexture, TexCoords);

		// Invert colors
    //FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    // Average grayscale
		//FragColor = texture(screenTexture, TexCoords);
		//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
		//FragColor = vec4(average, average, average, 1.0);

		// Weighted average grayscale
		//FragColor = texture(screenTexture, TexCoords);
		//float average = (0.2126*FragColor.r + 0.7152*FragColor.g + 0.0722*FragColor.b);
		//FragColor = vec4(average, average, average, 1.0);

		// Contrast
		FragColor = texture(screenTexture, TexCoords);
		FragColor = vec4((FragColor.rgb - 0.5) * (1.0 + contrast) + 0.5, FragColor.a);
}

