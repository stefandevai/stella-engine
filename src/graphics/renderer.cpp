#include "stella/graphics/renderer.h"

#include <cstddef>
#include <algorithm>

// Temporary includes
#include <iostream>

namespace stella { namespace graphics { 
  Renderer::Renderer()
  {
    this->init();
  }

  Renderer::~Renderer()
  {
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
    glDeleteVertexArrays(1, &this->VAO);
  }

  void Renderer::Begin()
  {
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    VertexBuffer = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  }

  void Renderer::Submit(const Sprite &sprite)
  {
    const glm::vec2 &position = sprite.GetPos();
    const glm::vec2 &dimensions = sprite.GetDimensions();
    const glm::vec4 &color = sprite.GetColor();
    
    const glm::vec2 &uv = sprite.GetFrameCoords();
		const SpriteSheet &spritesheet = sprite.GetSpriteSheet();
    const GLuint &stW = spritesheet.GetWidth();
    const GLuint &stH = spritesheet.GetHeight();

		Texture *texture = sprite.GetTexture();
    GLfloat texid = -1.0f;

		GLboolean found = false;
		for (unsigned int i = 0; i < Textures.size() && !found; ++i) 
		{
			if (Textures[i] == texture)
			{
				found = true;
				texid = (GLfloat)i;
				break;
			}
		}
    
		if (!found)
		{
			Textures.push_back(texture);
			texid = (GLfloat)(Textures.size() - 1);
		}

    int r = color.x * 255.0f;
    int g = color.y * 255.0f;
    int b = color.z * 255.0f;
    int a = color.w * 255.0f;

    unsigned int c = a << 24 | b << 16 | g << 8 | r;

    GLfloat uvoffsetX = dimensions.x/(GLfloat)stW;
    GLfloat uvoffsetY = dimensions.y/(GLfloat)stH;

    this->VertexBuffer->vertex = glm::vec3(position, 1.0f);
    this->VertexBuffer->uv = glm::vec2(uv.x, uv.y);
    this->VertexBuffer->tid = texid;
    this->VertexBuffer->color = c;
    this->VertexBuffer++;
    
    this->VertexBuffer->vertex = glm::vec3(position.x + dimensions.x, position.y, 1.0f);
    this->VertexBuffer->uv = glm::vec2(uv.x + uvoffsetX, uv.y);
    this->VertexBuffer->tid = texid;
    this->VertexBuffer->color = c;
    this->VertexBuffer++;
    
    this->VertexBuffer->vertex = glm::vec3(position.x + dimensions.x, position.y + dimensions.y, 1.0f);
    this->VertexBuffer->uv = glm::vec2(uv.x + uvoffsetX, uv.y - uvoffsetY);
    this->VertexBuffer->tid = texid;
    this->VertexBuffer->color = c;
    this->VertexBuffer++;
    
    this->VertexBuffer->vertex = glm::vec3(position.x, position.y + dimensions.y, 1.0f);
    this->VertexBuffer->uv = glm::vec2(uv.x, uv.y - uvoffsetY);
    this->VertexBuffer->tid = texid;
    this->VertexBuffer->color = c;
    this->VertexBuffer++;

    this->IndexCount += 6;
  }

  void Renderer::End()
  {
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Renderer::Draw()
  {
		for (unsigned int i = 0; i < Textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			Textures[i]->Bind();
		}

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->IndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		this->IndexCount = 0;
  }

  void Renderer::init()
  {
    this->IndexCount = 0;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*)0);
    glEnableVertexAttribArray(VERTEX_INDEX);

    glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) offsetof(VertexData, uv));
    glEnableVertexAttribArray(UV_INDEX);

    glVertexAttribPointer(TID_INDEX, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) offsetof(VertexData, tid));
    glEnableVertexAttribArray(TID_INDEX);

    glVertexAttribPointer(COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE, (GLvoid*) offsetof(VertexData, color));
    glEnableVertexAttribArray(COLOR_INDEX);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint offset = 0;
    GLuint indices[INDICES_SIZE];
    for (GLint i = 0; i < INDICES_SIZE; i+=6) {
      indices[i] = offset;
      indices[i+1] = offset + 1;
      indices[i+2] = offset + 2;

      indices[i+3] = offset;
      indices[i+4] = offset + 2;
      indices[i+5] = offset + 3;

      offset += 4;
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
  }
} }

