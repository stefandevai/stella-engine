#pragma once

#include "stella/graphics/vertex.hpp"
#include "stella/graphics/renderer/renderable.hpp"
#include "stella/core/resource/asset.hpp"
#include <vector>

namespace stella
{
namespace graphics
{
  class ShaderProgram;
  class Texture;

  using TextureVector = std::vector<std::shared_ptr<Texture>>;

  class Mesh : public core::Asset, public Renderable
  {
  public:
    Mesh (const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const TextureVector& textures);
    ~Mesh();

    void render (ShaderProgram& shader);

  private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
    const std::vector<Vertex> m_vertices;
    const std::vector<uint32_t> m_indices;
    const TextureVector m_textures;

  private:
    void m_setup();
  };
} // namespace graphics
} // namespace stella
