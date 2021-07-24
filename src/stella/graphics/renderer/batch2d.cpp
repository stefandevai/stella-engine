#undef NDEBUG

#include "stella/graphics/renderer/batch2d.hpp"
#include "stella/graphics/texture2.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/position2.hpp"
#include "stella/components/color.hpp"
#include "stella/components/transform.hpp"
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export
#include <spdlog/spdlog.h>

namespace stella::graphics
{
Batch2D::Batch2D()
{
  glGenVertexArrays (1, &m_vao);
  glGenBuffers (1, &m_vbo);
  glGenBuffers (1, &m_ebo);

  glBindVertexArray (m_vao);
  glBindBuffer (GL_ARRAY_BUFFER, m_vbo);

  glBufferData (GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
  glVertexAttribPointer (POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) 0);
  glEnableVertexAttribArray (POSITION_INDEX);

  glVertexAttribPointer (TEXCOORDS_INDEX, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, texcoords));
  glEnableVertexAttribArray (TEXCOORDS_INDEX);

  glVertexAttribPointer (TEXTURE_ID_INDEX, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, texture_id));
  glEnableVertexAttribArray (TEXTURE_ID_INDEX);

  glVertexAttribPointer (COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE, (GLvoid*) offsetof (VertexData, color));
  glEnableVertexAttribArray (COLOR_INDEX);

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
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

  glBindVertexArray (0);
}

Batch2D::~Batch2D()
{
  glDeleteBuffers (1, &m_ebo);
  glDeleteBuffers (1, &m_vbo);
  glDeleteVertexArrays (1, &m_vao);
}

void Batch2D::render (ShaderProgram& shader)
{
  // Return early if no sprites were added to the batch
  if (m_index_count == 0)
  {
    return;
  }

  for (unsigned int i = 0; i < m_textures.size(); ++i)
  {
    glActiveTexture (GL_TEXTURE0 + i);
    m_textures[i]->bind();
    shader.set_int ("textures[" + std::to_string(i) + "]", i);
  }
  glActiveTexture (GL_TEXTURE0);

  glBindVertexArray (m_vao);
  glDrawElements (GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray (0);
  m_index_count = 0;
}

void Batch2D::emplace (entt::registry& registry, entt::entity entity)
{
  const auto& sprite                       = registry.get<component::Sprite> (entity);
  const auto& position                     = registry.get<component::Position2> (entity);
  const glm::vec2 size                     = sprite.get_size();
  const std::array<glm::vec2, 4> texcoords = sprite.get_texcoords();
  unsigned int color                       = 4294967295; // Default white color

  /* spdlog::warn("{} {} {} {}", texcoords[0].x, texcoords[1].x, texcoords[2].x, texcoords[3].x); */
  /* spdlog::warn("{} {} {} {}", texcoords[0].y, texcoords[1].y, texcoords[2].y, texcoords[3].y); */

  const std::shared_ptr<Texture>& texture = sprite.texture;

  assert (sprite.texture != nullptr);
  assert (size.x != 0);
  assert (size.y != 0);

  if (registry.has<component::Color> (entity))
  {
    const auto& color_component = registry.get<component::Color> (entity);
    color                       = color_component.int_color;
  }

  // Build vector of textures to bind when rendering
  // texture_index is the index in m_textures that will
  // be translated to a index in the shader.
  int texture_index = 0;
  const auto it     = std::find (m_textures.begin(), m_textures.end(), texture);
  if (it == m_textures.end())
  {
    texture_index = m_textures.size();
    m_textures.emplace_back (texture);
  }
  else
  {
    texture_index = it - m_textures.begin();
  }

  // Get transformations and apply them to the sprite vertices
  auto general_transform = glm::mat4 (1.0f);
  if (registry.has<component::Transform> (entity))
  {
    const auto& transform_component = registry.get<component::Transform> (entity);

    // Translate and add pivot for other transformations
    general_transform = glm::translate (general_transform, glm::vec3 (position.x + transform_component.pivot.x, position.y + transform_component.pivot.y, position.z + transform_component.pivot.z));

    // Scale only if necessary
    if (transform_component.scale.x != 1.f || transform_component.scale.y != 1.f || transform_component.scale.z != 1.f)
    {
      general_transform = glm::scale (general_transform, transform_component.scale);
    }

    // Rotate only if necessary
    if (transform_component.rotation.x != 0.0f)
    {
      general_transform = glm::rotate (general_transform, glm::radians (transform_component.rotation.x), glm::vec3 (1.f, 0.f, 0.f));
    }
    if (transform_component.rotation.y != 0.0f)
    {
      general_transform = glm::rotate (general_transform, glm::radians (transform_component.rotation.y), glm::vec3 (0.f, 1.f, 0.f));
    }
    if (transform_component.rotation.z != 0.0f)
    {
      general_transform = glm::rotate (general_transform, glm::radians (transform_component.rotation.z), glm::vec3 (0.f, 0.f, 1.f));
    }

    // Remove the pivot translation if needed
    if (transform_component.pivot.x != 0.f || transform_component.pivot.y != 0.f || transform_component.pivot.z != 0.f)
    {
      general_transform = glm::translate (general_transform, transform_component.pivot * -1.f);
    }
  }
  else
  {
    general_transform = glm::translate (general_transform, glm::vec3 (position.x, position.y, position.z));
  }

  // Top left vertex
  glm::vec4 transformation_result = general_transform * glm::vec4 (0.f, 0.f, 1.f, 1.f);
  m_vertex_buffer->position       = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
  m_vertex_buffer->texcoords      = texcoords[0];
  m_vertex_buffer->texture_id     = texture_index;
  m_vertex_buffer->color          = color;
  m_vertex_buffer++;

  // Top right vertex
  transformation_result       = general_transform * glm::vec4 (size.x, 0.f, 1.f, 1.f);
  m_vertex_buffer->position   = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
  m_vertex_buffer->texcoords  = texcoords[1];
  m_vertex_buffer->texture_id = texture_index;
  m_vertex_buffer->color      = color;
  m_vertex_buffer++;

  // Bottom right vertex
  transformation_result       = general_transform * glm::vec4 (size.x, size.y, 1.f, 1.f);
  m_vertex_buffer->position   = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
  m_vertex_buffer->texcoords  = texcoords[2];
  m_vertex_buffer->texture_id = texture_index;
  m_vertex_buffer->color      = color;
  m_vertex_buffer++;

  // Bottom left vertex
  transformation_result       = general_transform * glm::vec4 (0.f, size.y, 1.f, 1.f);
  m_vertex_buffer->position   = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
  m_vertex_buffer->texcoords  = texcoords[3];
  m_vertex_buffer->texture_id = texture_index;
  m_vertex_buffer->color      = color;
  m_vertex_buffer++;

  // Each quad has 6 vertices, we have therefore to increment by 6 each time
  m_index_count += 6;
}

void Batch2D::init_emplacing()
{
  glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
  m_vertex_buffer = static_cast<VertexData*> (glMapBuffer (GL_ARRAY_BUFFER, GL_WRITE_ONLY));

  if (m_vertex_buffer == nullptr)
  {
    throw std::runtime_error ("Could not initialize VBO for Batch2D");
  }
}

void Batch2D::finalize_emplacing()
{
  glUnmapBuffer (GL_ARRAY_BUFFER);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
}

} // namespace stella::graphics
