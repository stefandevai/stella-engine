#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 lightPos;

void main()
{
    vec3 light_pos = vec3(lightPos.x, lightPos.y, lightPos.z);
    vec4 object_color = texture(ourTexture, TexCoord);
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    float ambient_strength = 0.1;
    vec3 ambient = light_color*ambient_strength;

    vec3 normal = Normal;
    vec3 light_dir = normalize(light_pos - FragPos);
    vec3 diffuse = max(dot(normal, light_dir), 0.0)*light_color;

    vec4 final_color = vec4(object_color.xyz*(ambient + diffuse), object_color.a);
    FragColor = final_color;
}