#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(aPos, 1.0);
    // Invert y tex coord
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);

    // DO THIS CALC OUTSIDE THE SHADER
    // USE IF PERSPECTIVE
     Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // USE IF ORTHO
    //Normal = normalize(vec3(mvp * vec4(aNormal, 1.0)));
    
    FragPos = vec3(model * vec4(aPos, 1.0));
}
