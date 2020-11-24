#define TINYOBJLOADER_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include "stella/systems/render2.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/vertex.hpp"
#include "stella/graphics/texture2.hpp"
#include "stella/graphics/camera.hpp"
#include "stella/core/texture_loader.hpp"
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
    std::string filename = "/Users/stefandevai/Developer/games/stella-engine/nikte2/assets/models/skull/12140_Skull_v3_L2.obj";
    std::string basepath = "/Users/stefandevai/Developer/games/stella-engine/nikte2/assets/models/skull/";
    const bool triangulate = true;
  
    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, filename.c_str(), basepath.c_str(), triangulate))
    {
      throw std::runtime_error ("Could not load object: " + warning + error);
    }

    std::vector<uint32_t> indices;
    std::vector<graphics::Vertex> vertices;
    std::vector<std::shared_ptr<graphics::Texture>> textures;
    std::unordered_map<graphics::Vertex, uint32_t> unique_vertices;

    for (const auto& shape : shapes)
    {
      for (const auto& index : shape.mesh.indices)
      {
        glm::vec3 position{1.0f};
        if (attributes.vertices.size() > 0)
        {
          position =
          {
            attributes.vertices[3 * index.vertex_index + 0],
            attributes.vertices[3 * index.vertex_index + 1],
            attributes.vertices[3 * index.vertex_index + 2],
          };
        }

        glm::vec3 normal{1.0f};
        if (attributes.normals.size() > 0)
        {
          normal = 
          {
            attributes.normals[3 * index.normal_index + 0],
            attributes.normals[3 * index.normal_index + 1],
            attributes.normals[3 * index.normal_index + 2],
          };
        }

        glm::vec2 texcoords{1.0f};
        if (attributes.texcoords.size() > 0)
        {
          texcoords =
          {
            attributes.texcoords[2 * index.texcoord_index + 0],
            1.0f - attributes.texcoords[2 * index.texcoord_index + 1],
          };
        }

        const graphics::Vertex vertex{position, normal, texcoords};
        if (unique_vertices.count(vertex) == 0)
        {
          unique_vertices.emplace (vertex, static_cast<uint32_t>(vertices.size()));
          vertices.emplace_back (vertex);
        }

        indices.emplace_back(unique_vertices.at(vertex));
      }

      //// TODO: Check if object/group has more than one material, right now we use only the first
      //if (materials.size() > 0 && shape.mesh.material_ids.size() > 0)
      //{
        //const auto material_id = shape.mesh.material_ids[0];
        //const auto& material = materials[material_id];
        //m_asset_manager.add<core::TextureLoader>(material.diffuse_texname, basepath + material.diffuse_texname, graphics::TextureType::DIFFUSE);
        //textures.emplace_back (m_asset_manager.get<graphics::Texture>(material.diffuse_texname));
      //}
    }

    // ===========================
    // Material loading
    // ===========================
    // TODO: Check if object/group has more than one material, right now we use only the first
    for (const auto& material : materials)
    {
      if (!material.diffuse_texname.empty())
      {
        m_asset_manager.add<core::TextureLoader>(material.diffuse_texname, basepath + material.diffuse_texname, graphics::TextureType::DIFFUSE);
        textures.emplace_back (m_asset_manager.get<graphics::Texture>(material.diffuse_texname));
      }
    }

    meshes.emplace_back(std::make_shared<graphics::Mesh>(vertices, indices, textures));

    m_shader_program = m_asset_manager.get<graphics::ShaderProgram>("simple-shader");
  }

  float accumulator = 0.0f;
  void Render::render (entt::registry& registry, const graphics::Camera& camera, const double dt)
  {
    //m_texture->bind();
    accumulator += static_cast<float>(dt);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    model = glm::rotate(model, accumulator, glm::vec3(0.9f, 0.0f, 0.0f));

    m_shader_program->set_mat_4("mvp", camera.get_projection_matrix() * camera.get_view_matrix() * model);
    m_shader_program->use();

    for (auto& mesh : meshes)
    {
      mesh->draw(*m_shader_program);
    }
  }

}
}
