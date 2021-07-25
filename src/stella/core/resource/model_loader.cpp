#define TINYOBJLOADER_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include "stella/core/resource/model_loader.hpp"
#include "stella/core/resource/texture_loader.hpp"
#include "stella/core/resource/asset_manager.hpp"
#include "stella/graphics/renderer/mesh.hpp"
#include "stella/graphics/vertex.hpp"
#include "stella/graphics/texture2.hpp"
/* #include "../../../lib/tinyobjloader/tinyobjloader.hpp" */
#include <tiny_obj_loader.h>
#include <filesystem>
#include <unordered_map>

namespace stella
{
namespace core
{
  ModelLoader::ModelLoader (const std::string& filepath, AssetManager* asset_manager) : AssetLoader (AssetType::MODEL), m_filepath (filepath), m_asset_manager (asset_manager) {}

  std::shared_ptr<Asset> ModelLoader::construct()
  {
    std::vector<std::shared_ptr<graphics::Mesh>> meshes;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning;
    std::string error;
    std::filesystem::path basedir = std::filesystem::path (m_filepath).remove_filename();
    const bool triangulate        = true;

    if (!tinyobj::LoadObj (&attributes, &shapes, &materials, &warning, &error, m_filepath.c_str(), basedir.c_str(), triangulate))
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
          position = {
              attributes.vertices[3 * index.vertex_index + 0],
              attributes.vertices[3 * index.vertex_index + 1],
              attributes.vertices[3 * index.vertex_index + 2],
          };
        }

        glm::vec3 normal{1.0f};
        if (attributes.normals.size() > 0)
        {
          normal = {
              attributes.normals[3 * index.normal_index + 0],
              attributes.normals[3 * index.normal_index + 1],
              attributes.normals[3 * index.normal_index + 2],
          };
        }

        glm::vec2 texcoords{1.0f};
        if (attributes.texcoords.size() > 0)
        {
          texcoords = {
              attributes.texcoords[2 * index.texcoord_index + 0],
              1.0f - attributes.texcoords[2 * index.texcoord_index + 1],
          };
        }

        const graphics::Vertex vertex{position, normal, texcoords};
        if (unique_vertices.count (vertex) == 0)
        {
          unique_vertices.emplace (vertex, static_cast<uint32_t> (vertices.size()));
          vertices.emplace_back (vertex);
        }

        indices.emplace_back (unique_vertices.at (vertex));
      }
    }

    // ===========================
    // Material loading
    // ===========================
    // TODO: Check if object/group has more than one material, right now we use only the first
    for (const auto& material : materials)
    {
      if (!material.diffuse_texname.empty())
      {
        m_asset_manager->add<core::TextureLoader> (material.diffuse_texname, basedir.string() + material.diffuse_texname, graphics::TextureType::DIFFUSE);
        textures.emplace_back (m_asset_manager->get<graphics::Texture> (material.diffuse_texname));
      }

      if (!material.specular_texname.empty())
      {
        m_asset_manager->add<core::TextureLoader> (material.specular_texname, basedir.string() + material.specular_texname, graphics::TextureType::SPECULAR);
        textures.emplace_back (m_asset_manager->get<graphics::Texture> (material.specular_texname));
      }

      if (!material.normal_texname.empty())
      {
        m_asset_manager->add<core::TextureLoader> (material.normal_texname, basedir.string() + material.normal_texname, graphics::TextureType::NORMAL);
        textures.emplace_back (m_asset_manager->get<graphics::Texture> (material.normal_texname));
      }
    }

    return std::make_shared<graphics::Mesh> (vertices, indices, textures);
  }

} // namespace core
} // namespace stella
