#include "stella/graphics/sprite_fog_renderer.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include "stella/graphics/sprite.hpp"
#include "stella/graphics/texture.hpp"
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace stella
{
namespace graphics
{
  //std::vector<graphics::Texture*> SpriteFogRenderer::Textures = std::vector<graphics::Texture*>();

  SpriteFogRenderer::SpriteFogRenderer()
  {
    this->TransformationStack.push_back (glm::mat4());
    this->TransformationBack = &this->TransformationStack.back();
    this->TexturesBinded     = false;
    this->init();
  }

  SpriteFogRenderer::~SpriteFogRenderer()
  {
    glDeleteBuffers (1, &this->VBO);
    glDeleteBuffers (1, &this->EBO);
    glDeleteVertexArrays (1, &this->VAO);
  }

  void SpriteFogRenderer::init()
  {
    this->IndexCount = 0;
    glGenVertexArrays (1, &this->VAO);
    glGenBuffers (1, &this->VBO);
    glGenBuffers (1, &this->EBO);

    glBindVertexArray (this->VAO);
    glBindBuffer (GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glBufferData (GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer (VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) 0);
    glEnableVertexAttribArray (VERTEX_INDEX);

    glVertexAttribPointer (UV_INDEX, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, uv));
    glEnableVertexAttribArray (UV_INDEX);

    glVertexAttribPointer (TID_INDEX, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, tid));
    glEnableVertexAttribArray (TID_INDEX);

    glVertexAttribPointer (
        YORIGIN_INDEX, 1, GL_FLOAT, GL_TRUE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, yorigin));
    glEnableVertexAttribArray (YORIGIN_INDEX);

    glVertexAttribPointer (
        HEIGHT_INDEX, 1, GL_FLOAT, GL_TRUE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, height));
    glEnableVertexAttribArray (HEIGHT_INDEX);

    glBindBuffer (GL_ARRAY_BUFFER, 0);

    GLint offset = 0;
    GLuint indices[INDICES_SIZE];
    for (GLuint i = 0; i < INDICES_SIZE; i += 6)
    {
      indices[i]     = offset;
      indices[i + 1] = offset + 1;
      indices[i + 2] = offset + 2;

      indices[i + 3] = offset;
      indices[i + 4] = offset + 2;
      indices[i + 5] = offset + 3;

      offset += 4;
    }
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

    glBindVertexArray (0);
  }

  void SpriteFogRenderer::Begin()
  {
    glBindBuffer (GL_ARRAY_BUFFER, this->VBO);
    VertexBuffer = static_cast<VertexData*> (glMapBuffer (GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  }

  void SpriteFogRenderer::Submit (const std::shared_ptr<Renderable> renderable)
  {
    auto spr = std::dynamic_pointer_cast<Sprite> (renderable);
    if (spr != nullptr)
    {
      Submit (spr);
    }
  }

  void SpriteFogRenderer::Submit (const std::shared_ptr<Sprite> sprite)
  {
    const glm::vec3& position   = sprite->GetPos();
    const glm::vec2& dimensions = sprite->GetDimensions();
    const float rotation        = sprite->GetRotation();
    const glm::vec2& scale      = sprite->GetScale();
    const float yorigin         = position.y;
    const float height         = position.y + dimensions.y;

    const glm::vec2& uv            = sprite->GetFrameCoords();
    const SpriteSheet& spritesheet = sprite->GetSpriteSheet();
    const GLuint stW               = spritesheet.GetWidth();
    const GLuint stH               = spritesheet.GetHeight();

    Texture* texture = sprite->GetTexture();

    if (!texture->IsCached())
    {
      // std::cout << texture->GetID() << '\n';
      Textures.push_back (texture);
      texture->SetCached ((GLfloat) (Textures.size() - 1));
      this->TexturesBinded = false;
    }

    GLfloat uvoffsetX = dimensions.x / (GLfloat) stW;
    GLfloat uvoffsetY = dimensions.y / (GLfloat) stH;

    auto particular_transform = *this->TransformationBack;
    particular_transform      = glm::translate (
        particular_transform,
        glm::vec3 (position + glm::vec3 (dimensions, 0.f) / 2.f)); // Translating half dimension to set the point of
                                                                        // rotation to the center of the sprite
    particular_transform = glm::scale (particular_transform, glm::vec3 (scale, 1.f));
    particular_transform = glm::rotate (particular_transform, glm::radians (rotation), glm::vec3 (0.f, 0.f, 1.f));
    particular_transform = glm::translate (particular_transform,
                                           glm::vec3 (-dimensions / 2.f,
                                                      0.f)); // Removing the added half dimension

    // particular_transform = glm::translate(particular_transform, position); //
    // Translating half dimension to set the point of rotation to the center of
    // the sprite particular_transform = glm::scale(particular_transform,
    // glm::vec3(scale, 1.f)); particular_transform =
    // glm::translate(particular_transform, glm::vec3(dimensions/2.f, 0.f)); //
    // Translating half dimension to set the point of rotation to the center of
    // the sprite particular_transform = glm::rotate(particular_transform,
    // glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)); particular_transform =
    // glm::translate(particular_transform, glm::vec3(-dimensions/2.f, 0.f));

    auto transformation_result = particular_transform * glm::vec4 (0.f, 0.f, 1.f, 1.f);
    this->VertexBuffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    this->VertexBuffer->uv     = glm::vec2 (uv.x, uv.y);
    this->VertexBuffer->tid    = texture->GetCacheID();
    this->VertexBuffer->yorigin  = yorigin;
    this->VertexBuffer->height  = height;
    this->VertexBuffer++;

    transformation_result      = particular_transform * glm::vec4 (dimensions.x, 0.f, 1.f, 1.f);
    this->VertexBuffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    this->VertexBuffer->uv     = glm::vec2 (uv.x + uvoffsetX, uv.y);
    this->VertexBuffer->tid    = texture->GetCacheID();
    this->VertexBuffer->yorigin  = yorigin;
    this->VertexBuffer->height  = height;
    this->VertexBuffer++;

    transformation_result      = particular_transform * glm::vec4 (dimensions.x, dimensions.y, 1.f, 1.f);
    this->VertexBuffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    this->VertexBuffer->uv     = glm::vec2 (uv.x + uvoffsetX, uv.y - uvoffsetY);
    this->VertexBuffer->tid    = texture->GetCacheID();
    this->VertexBuffer->yorigin  = yorigin;
    this->VertexBuffer->height  = height;
    this->VertexBuffer++;

    transformation_result      = particular_transform * glm::vec4 (0.f, dimensions.y, 1.f, 1.f);
    this->VertexBuffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    this->VertexBuffer->uv     = glm::vec2 (uv.x, uv.y - uvoffsetY);
    this->VertexBuffer->tid    = texture->GetCacheID();
    this->VertexBuffer->yorigin  = yorigin;
    this->VertexBuffer->height  = height;
    this->VertexBuffer++;

    this->IndexCount += 6;
  }

  void SpriteFogRenderer::End()
  {
    glUnmapBuffer (GL_ARRAY_BUFFER);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
  }

  void SpriteFogRenderer::Draw()
  {
    for (unsigned int i = 0; i < Textures.size(); ++i)
    {
      glActiveTexture (GL_TEXTURE0 + i);
      Textures[i]->Bind();
    }

    glBindVertexArray (this->VAO);
    glDrawElements (GL_TRIANGLES, this->IndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray (0);
    this->IndexCount = 0;
  }
} // namespace graphics
} // namespace stella