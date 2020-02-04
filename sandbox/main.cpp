#include "display.h"
#include "shader.h"
#include "texture.h"
#include "../lib/glad/include/glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using Display = stella::graphics::Display;
using Shader = stella::graphics::Shader;
using Texture = stella::graphics::Texture;

const unsigned int SCR_WIDTH = 896;
const unsigned int SCR_HEIGHT = 504;

int main(void)
{
    // ********************
    // ** INITIALIZATION **
    // ********************
    Display display{SCR_WIDTH, SCR_HEIGHT, "Lighting test"};
    Shader shader{"./assets/lighting.vert", "./assets/lighting.frag"};
    Shader light_shader{"./assets/basic.vert", "./assets/basic.frag"};
    Texture texture{"assets/church_sprite.png", true};
    Texture texture_normal{"assets/church_normal.png", true};
    Texture light_texture{"assets/light_texture.png", true};

    display.SetClearColor(0.1f, 0.1f, 0.1f);

    // Use orthographic projection if true, perspective if false
    bool use_ortho = true;
    // Draw quad if true, draw cube if false
    bool use_quad = true;

    // ***************
    // ** QUAD DATA **
    // ***************
    float qvertices[] = {
        // positions         // texture coords
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int qVBO, qVAO, qEBO;
    glGenVertexArrays(1, &qVAO);
    glGenBuffers(1, &qVBO);
    glGenBuffers(1, &qEBO);

    glBindVertexArray(qVAO);

    glBindBuffer(GL_ARRAY_BUFFER, qVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(qvertices), qvertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, qEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ***************
    // ** CUBE DATA **
    // ***************
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ****************
    // ** LIGHT DATA **
    // ****************
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, qVBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, qEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ***************
    // ** MAIN LOOP **
    // ***************
    glm::mat4 projection    = glm::mat4(1.0f);
    if (use_ortho)
    {
        projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 100.0f);    
    }
    else
    {
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    }
    shader.Enable();
    shader.SetMat4("projection", projection);
    shader.SetInt("ourTexture", 0);
    shader.SetInt("normalMap", 1);
    light_shader.Enable();
    //light_shader.SetMat4("projection", projection);
    light_shader.SetMat4("projection", glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 100.0f));

    while (display.IsRunning())
    {
        // *****************
        // ** PREPARATION **
        // *****************
        display.Clear();
        shader.Enable();

        //auto current_time = display.GetTime();
        double mouse_pos_x, mouse_pos_y;
        display.GetMousePos(mouse_pos_x, mouse_pos_y);
        glm::vec3 light_pos{mouse_pos_x, mouse_pos_y, 1.5f};
        // glm::vec3 light_pos{mouse_pos_x/(float)SCR_WIDTH, mouse_pos_y/(float)SCR_HEIGHT, 4.0f};
        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          = glm::mat4(1.0f);
        //float ortho_scale = 300.f;
        float view_zoom = -2.f;

        if (use_ortho)
        {
            // model = glm::scale(model, glm::vec3(ortho_scale, ortho_scale, 1.0));
            model = glm::scale(model, glm::vec3(536.0f, 258.0f, 1.0));
            view = glm::translate(view, glm::vec3((float)SCR_WIDTH/2.f, (float)SCR_HEIGHT/2.f, view_zoom));
            // shader.SetVec3f("lightPos", glm::vec3(light_pos.x - 448.f, -light_pos.y + 252.f, -light_pos.z*50.0f));
            shader.SetVec3f("lightPos", glm::vec3(light_pos.x - 448.f, -light_pos.y + 252.f, 400.0f));
            shader.SetVec3f("viewPos", glm::vec3(0.0f, 0.0f, -light_pos.z*50.0f));
        }
        else
        {
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, view_zoom));
            shader.SetVec3f("lightPos", glm::vec3((light_pos.x - 448.0f)/(float)SCR_WIDTH, (-light_pos.y + 252.f)/(float)SCR_HEIGHT, 1.0f));
        }
        //model = glm::rotate(model, 0.01f*current_time*glm::radians(5.0f), glm::vec3(1.0f, 0.5f, 0.3f));  
        
        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        
        glActiveTexture(GL_TEXTURE0);
        texture.Bind();

        glActiveTexture(GL_TEXTURE1);
        texture_normal.Bind();

        // ***************
        // ** DRAW CUBE **
        // ***************
        if (!use_quad)
        {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ***************
        // ** DRAW QUAD **
        // ***************
        else
        {
            glBindVertexArray(qVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // ***********************
        // ** DRAW LIGHT SOURCE **
        // ***********************
        glm::mat4 light_model         = glm::mat4(1.0f);
        //glm::mat4 light_view          = glm::mat4(1.0f);

        
        if (use_ortho)
        {
            light_model = glm::translate(light_model, glm::vec3(light_pos.x - 448.f, -light_pos.y + 252.f, light_pos.z));
            light_model = glm::scale(light_model, glm::vec3(32.0f, 32.0f, 1.0));
        }
        else
        {
            light_model = glm::translate(light_model, glm::vec3(light_pos.x, -light_pos.y + 504.f, light_pos.z));
            light_model = glm::scale(light_model, glm::vec3(32.0f, 32.0f, 1.0));
        }

        light_shader.Enable();
        light_shader.SetMat4("model", light_model);
        light_shader.SetMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        light_texture.Bind();
        
        glBindVertexArray(lightVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        display.Update();
    }

    // **************
    // ** CLEANING **
    // **************
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &qVAO);
    glDeleteBuffers(1, &qVBO);
    glDeleteBuffers(1, &qEBO);
    return 0;
}