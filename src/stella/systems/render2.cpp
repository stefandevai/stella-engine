#define TINYOBJLOADER_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include "stella/systems/render2.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/texture2.hpp"
#include "stella/graphics/camera.hpp"
#include "../../../lib/tinyobjloader/tinyobjloader.hpp"
#include <spdlog/spdlog.h>
#include <entt/entity/registry.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace stella
{
namespace system
{
  Render::Render (core::AssetManager& asset_manager)
    : System ("render"), m_asset_manager (asset_manager)
  {
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning;
    std::string error;
    std::string filename = "/Users/stefandevai/Developer/games/stella-engine/nikte2/assets/models/backpack/backpack.obj";
    std::string basepath = "/Users/stefandevai/Developer/games/stella-engine/nikte2/assets/models/backpack/";
    const bool triangulate = true;
  
    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, filename.c_str(), basepath.c_str(), triangulate))
    {
      throw std::runtime_error ("Could not load object: " + warning + error);
    }

    for (const auto& shape : shapes)
    {
      std::vector<uint32_t> indices;
      std::vector<graphics::Vertex> vertices;
      std::unordered_map<glm::vec3, unsigned int> unique_vertices;

      for (const auto& index : shape.mesh.indices)
      {
        glm::vec3 position
        {
          position.x = attributes.vertices[3 * index.vertex_index + 0],
          position.y = attributes.vertices[3 * index.vertex_index + 1],
          position.z = attributes.vertices[3 * index.vertex_index + 2],
        };

        if (unique_vertices.count(position) == 0)
        {
          unique_vertices.emplace (position, static_cast<uint32_t>(vertices.size()));
          vertices.emplace_back(graphics::Vertex{position});
        }

        indices.emplace_back(unique_vertices.at(position));
      }

      meshes.emplace_back(std::make_shared<graphics::Mesh>(vertices, indices));
    }

    m_shader_program = m_asset_manager.get<graphics::ShaderProgram>("simple-shader");
  }

  float accumulator = 0.0f;
  void Render::render (entt::registry& registry, const graphics::Camera& camera, const double dt)
  {
    //m_texture->bind();
    accumulator += static_cast<float>(dt);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
    model = glm::rotate(model, accumulator, glm::vec3(0.9f, 0.3f, 0.6f));

    m_shader_program->set_mat_4("model", model);
    m_shader_program->set_mat_4("view", camera.get_view_matrix());
    m_shader_program->set_mat_4("projection", camera.get_projection_matrix());

    m_shader_program->use();

    for (auto& mesh : meshes)
    {
      mesh->draw(*m_shader_program);
    }
  }

}
}
