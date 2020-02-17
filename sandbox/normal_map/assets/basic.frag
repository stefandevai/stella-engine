#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 lightColor;

void main()
{
    vec4 object_color = texture(ourTexture, TexCoord);
    vec4 final_color;
    if (object_color.a > 0) final_color = vec4(object_color.rgb * lightColor, object_color.a);  
    else final_color = object_color;
    FragColor = final_color;
}