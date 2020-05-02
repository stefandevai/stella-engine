#include "stella/graphics/sprite_renderer2.hpp"
#include "stella/graphics/texture.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include "stella/components/sprite2.hpp"
#include "stella/components/position.hpp"

#include <algorithm>
#include <cstddef>

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export
#include <iostream>

namespace stella
{
namespace graphics
{
  SpriteRendererT::SpriteRendererT() : RendererT()
  {
    m_transformation_stack.push_back (glm::mat4());
    m_transformation_back = &m_transformation_stack.back();
    this->textures_binded = false;
    this->init();
  }

  SpriteRendererT::~SpriteRendererT()
  {
    glDeleteBuffers (1, &this->m_VBO);
    glDeleteBuffers (1, &this->m_EBO);
    glDeleteVertexArrays (1, &this->m_VAO);
  }

  void SpriteRendererT::init()
  {
    m_index_count = 0;
    glGenVertexArrays (1, &this->m_VAO);
    glGenBuffers (1, &this->m_VBO);
    glGenBuffers (1, &this->m_EBO);

    glBindVertexArray (this->m_VAO);
    glBindBuffer (GL_ARRAY_BUFFER, this->m_VBO);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->m_EBO);

    glBufferData (GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer (VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) 0);
    glEnableVertexAttribArray (VERTEX_INDEX);

    glVertexAttribPointer (UV_INDEX, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, uv));
    glEnableVertexAttribArray (UV_INDEX);

    glVertexAttribPointer (TID_INDEX, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, tid));
    glEnableVertexAttribArray (TID_INDEX);

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

  void SpriteRendererT::begin()
  {
    glBindBuffer (GL_ARRAY_BUFFER, this->m_VBO);
    m_vertex_buffer = static_cast<VertexData*> (glMapBuffer (GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  }

  void SpriteRendererT::submit (entt::registry& registry, entt::entity entity)
  {
    auto& sprite = registry.get<component::SpriteT>(entity);
    auto& pos = registry.get<component::Position>(entity);

    const glm::vec3 position   = glm::vec3(pos.x, pos.y, pos.z);
    // const glm::vec2& dimensions = sprite->GetDimensions();
    const glm::vec2 dimensions = glm::vec2(32.0f, 32.0f);

    // const glm::vec2& uv            = sprite->GetFrameCoords();
    const glm::vec2 uv            = glm::vec2(0.0f, 0.0f);
    // const SpriteSheet& spritesheet = sprite->GetSpriteSheet();
    // const GLuint stW               = spritesheet.GetWidth();
    const GLuint stW               = 288;
    // const GLuint stH               = spritesheet.GetHeight();
    const GLuint stH               = 384;

    std::shared_ptr<Texture> texture = sprite.texture_ptr;

    if (!texture->IsCached())
    {
      // std::cout << texture->GetID() << '\n';
      m_textures.push_back (texture);
      texture->SetCached ((GLfloat) (m_textures.size() - 1));
      this->textures_binded = false;
    }

    GLfloat uvoffsetX = dimensions.x / (GLfloat) stW;
    GLfloat uvoffsetY = dimensions.y / (GLfloat) stH;

    auto particular_transform = *m_transformation_back;
    particular_transform      = glm::translate (
        particular_transform,
        glm::vec3 (position + glm::vec3 (dimensions, 0.f) / 2.f)); // Translating half dimension to set the point of
                                                                        // rotation to the center of the sprite
    // particular_transform = glm::scale (particular_transform, glm::vec3 (scale, 1.f));
    // particular_transform = glm::rotate (particular_transform, glm::radians (rotation), glm::vec3 (0.f, 0.f, 1.f));
    particular_transform = glm::translate (particular_transform,
                                           glm::vec3 (-dimensions / 2.f,
                                                      0.f)); // Removing the added half dimension

    auto transformation_result = particular_transform * glm::vec4 (0.f, 0.f, 1.f, 1.f);
    m_vertex_buffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    m_vertex_buffer->uv     = glm::vec2 (uv.x, uv.y);
    m_vertex_buffer->tid    = texture->GetCacheID();
    m_vertex_buffer++;

    transformation_result      = particular_transform * glm::vec4 (dimensions.x, 0.f, 1.f, 1.f);
    m_vertex_buffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    m_vertex_buffer->uv     = glm::vec2 (uv.x + uvoffsetX, uv.y);
    m_vertex_buffer->tid    = texture->GetCacheID();
    m_vertex_buffer++;

    transformation_result      = particular_transform * glm::vec4 (dimensions.x, dimensions.y, 1.f, 1.f);
    m_vertex_buffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    m_vertex_buffer->uv     = glm::vec2 (uv.x + uvoffsetX, uv.y - uvoffsetY);
    m_vertex_buffer->tid    = texture->GetCacheID();
    m_vertex_buffer++;

    transformation_result      = particular_transform * glm::vec4 (0.f, dimensions.y, 1.f, 1.f);
    m_vertex_buffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    m_vertex_buffer->uv     = glm::vec2 (uv.x, uv.y - uvoffsetY);
    m_vertex_buffer->tid    = texture->GetCacheID();
    m_vertex_buffer++;

    this->m_index_count += 6;
  }

  void SpriteRendererT::end()
  {
    glUnmapBuffer (GL_ARRAY_BUFFER);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
  }

  void SpriteRendererT::draw()
  {
    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
      glActiveTexture (GL_TEXTURE0 + i);
      m_textures[i]->Bind();
    }

    glBindVertexArray (m_VAO);
    glDrawElements (GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray (0);
    m_index_count = 0;
  }

} // namespace graphics
} // namespace stella
