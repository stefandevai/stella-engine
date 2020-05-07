#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform sampler2D normalMap;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

float cel_levels = 100.0;

void main()
{
    vec3 light_pos = vec3(lightPos.x, lightPos.y, lightPos.z);
    vec4 object_color = texture(ourTexture, TexCoord);

    // Normal
    vec3 normal = Normal;
    //vec3 normal = texture(normalMap, TexCoord).rgb;
    //vec3 normal = texture(normalMap, TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // Ambient
    float ambient_strength = 0.1;
    vec3 ambient = lightColor*ambient_strength;

    // Diffuse
    float diffuse_strength = 1.0;
    vec3 light_dir = normalize(light_pos - FragPos);
    vec3 diffuse = max(dot(normal, light_dir), 0.0)* lightColor * diffuse_strength;
    vec3 cel_level = floor(diffuse*cel_levels);
    diffuse = cel_level / cel_levels;

    // Specular
    float specular_strength = 0.0;
    vec3 view_direction = normalize(viewPos - FragPos);
    vec3 reflect_direction = reflect(light_dir, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32.0);
    vec3 specular = specular_strength * spec * lightColor;

    vec4 final_color = vec4(object_color.xyz*(ambient + diffuse + specular), object_color.a);
    FragColor = final_color;
    //FragColor = object_color;
}
