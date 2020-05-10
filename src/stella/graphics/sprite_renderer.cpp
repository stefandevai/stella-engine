#include "stella/graphics/sprite_renderer.hpp"
#include "stella/graphics/texture.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include "stella/components/sprite.hpp"
#include "stella/components/position.hpp"
#include "stella/components/color.hpp"
#include "stella/components/transform.hpp"
#include "stella/components/vertical.hpp"

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

    glVertexAttribPointer (
        COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, color));
    glEnableVertexAttribArray (COLOR_INDEX);

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
    auto& sprite               = registry.get<component::SpriteT> (entity);
    const auto& pos            = registry.get<component::Position> (entity);
    const glm::vec3 position   = glm::vec3 (pos.x, pos.y, pos.z);
    const glm::vec2 dimensions = glm::vec2 (sprite.get_width(), sprite.get_height());
    const glm::vec2 uv         = sprite.get_uv();
    unsigned int color         = 4294967295; // Default white color

    std::shared_ptr<Texture> texture = sprite.texture_ptr;

    if (registry.has<component::Color> (entity))
    {
      const auto& colorc = registry.get<component::Color> (entity);
      color              = colorc.int_color;
    }

    if (!texture->IsCached())
    {
      m_textures.push_back (texture);
      texture->SetCached ((GLfloat) (m_textures.size() - 1));
      this->textures_binded = false;
    }

    GLfloat uvoffsetX = dimensions.x / static_cast<GLfloat> (texture->GetWidth());
    GLfloat uvoffsetY = dimensions.y / static_cast<GLfloat> (texture->GetHeight());

    auto particular_transform = *m_transformation_back;
    if (registry.has<component::Vertical> (entity))
    {
      // Translation before scale and rotation
      // Also we multiply the height (dim.y) by sin(45deg) in order to compensate
      // the decrease in z position after rotation 
      particular_transform =
          glm::translate (particular_transform, glm::vec3(position.x, position.y - position.z, position.z + dimensions.y*0.70710678118f));
      
      // Scale in the y axis by 1 / (cos 45deg) in order to compensate for the scale reduction when rotating 
      particular_transform = glm::scale (particular_transform, glm::vec3(1.f, 1.41421356237f, 1.f));

      // Rotate -45deg in the x axis from the top in order to have depth information for lighting and other effects
      particular_transform =
          glm::rotate (particular_transform, glm::radians (-45.f), glm::vec3 (1.f, 0.f, 0.f));
    }

    else
    {
      auto trans = component::Transform();
      if (registry.has<component::Transform>(entity))
      {
        trans = registry.get<component::Transform> (entity);
      }
      // // Translating half dimension to set the point of rotation to the center of the sprite
      particular_transform =
          glm::translate (particular_transform, position + glm::vec3(dimensions.x, dimensions.y - position.z, 0.f)/2.f);
      particular_transform = glm::scale (particular_transform, trans.scale);
      particular_transform =
          glm::rotate (particular_transform, glm::radians (trans.rotation.x), glm::vec3 (1.f, 0.f, 0.f));
      particular_transform =
          glm::rotate (particular_transform, glm::radians (trans.rotation.y), glm::vec3 (0.f, 1.f, 0.f));
      particular_transform =
          glm::rotate (particular_transform, glm::radians (trans.rotation.z), glm::vec3 (0.f, 0.f, 1.f));
      // Removing the added half dimension
      particular_transform = glm::translate (particular_transform, glm::vec3(-dimensions.x, -dimensions.y, 0.f)/2.f);
      // std::cout << position.z << ' ' << (position + dim/2.f).z << '\n';
    }

    auto transformation_result = particular_transform * glm::vec4 (0.f, 0.f, 1.f, 1.f);
    m_vertex_buffer->vertex    = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    m_vertex_buffer->uv        = glm::vec2 (uv.x, uv.y);
    m_vertex_buffer->tid       = texture->GetCacheID();
    m_vertex_buffer->color     = color;
    m_vertex_buffer++;

    transformation_result   = particular_transform * glm::vec4 (dimensions.x, 0.f, 1.f, 1.f);
    m_vertex_buffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    m_vertex_buffer->uv     = glm::vec2 (uv.x + uvoffsetX, uv.y);
    m_vertex_buffer->tid    = texture->GetCacheID();
    m_vertex_buffer->color  = color;
    m_vertex_buffer++;

    transformation_result   = particular_transform * glm::vec4 (dimensions.x, dimensions.y, 1.f, 1.f);
    m_vertex_buffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    m_vertex_buffer->uv     = glm::vec2 (uv.x + uvoffsetX, uv.y - uvoffsetY);
    m_vertex_buffer->tid    = texture->GetCacheID();
    m_vertex_buffer->color  = color;
    m_vertex_buffer++;

    transformation_result   = particular_transform * glm::vec4 (0.f, dimensions.y, 1.f, 1.f);
    m_vertex_buffer->vertex = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
    m_vertex_buffer->uv     = glm::vec2 (uv.x, uv.y - uvoffsetY);
    m_vertex_buffer->tid    = texture->GetCacheID();
    m_vertex_buffer->color  = color;
    m_vertex_buffer++;

    // std::cout << transformation_result.x << '\n';
    // std::cout << position.x << '\n';
    // std::cout << "-----------" << '\n';

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
