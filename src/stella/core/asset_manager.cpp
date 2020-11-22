#include "stella/core/asset_manager.hpp"
#include "stella/core/texture_loader.hpp"
#include "stella/core/model_loader.hpp"
#include "stella/core/shader_loader.hpp"

#include "../../../lib/json/json.hpp"

namespace stella
{
namespace core
{

  AssetManager::AssetManager(const std::filesystem::path& filepath)
    : m_filepath(filepath)
  {
    // Get base dir from the config filepath
    m_base_dir = filepath.string();
    m_base_dir.remove_filename();

    // Load config file
    m_json.load (m_filepath);
    m_init_assets ();
  }

  template <typename T, typename... Args>
  void AssetManager::add (const std::string& name, const std::string& filepath, Args... args)
  {
    auto asset_loader = std::make_unique<T>(filepath, args...);
    std::weak_ptr<Asset> asset_ptr;
    assert(asset_loader != nullptr);
    assert(asset_ptr != nullptr);
    m_assets.emplace(name, std::make_pair(std::move(asset_ptr), std::move(asset_loader)));
  }

  //std::shared_ptr<Asset> AssetManager::get (const std::string& name)

  void AssetManager::m_init_assets()
  {
    if (m_json.object["assets"] == nullptr)
    {
      spdlog::warn("Project has no assets.");
      return;
    }

    auto assets = m_json.object["assets"].get<std::vector<nlohmann::json>>();
    
    for (auto& asset_info : assets)
    {
      auto name = asset_info["name"].get<std::string>();
      auto type_tag = asset_info["type"].get<std::string>();
      AssetType type = AssetType::NONE;

      try
      {
        type = m_asset_types.at(type_tag);
      }
      catch (std::out_of_range& e)
      {
        spdlog::warn("Unknown asset type {}.\n{}", type_tag, e.what());
        continue;
      }

      switch (type)
      {
        case AssetType::TEXTURE:
          {
            auto filepath = asset_info["path"].get<std::string>();
            auto full_path = m_base_dir / filepath;
            add<TextureLoader>(name, full_path);
          }
          break;

        case AssetType::MODEL:
          {
            auto filepath = asset_info["path"].get<std::string>();
            auto full_path = m_base_dir / filepath;
            // TODO: Implement model loading
            add<ModelLoader>(name, full_path);
          }
          break;

        case AssetType::SHADER:
          {
            auto vertex_filepath = asset_info["vertexFilepath"].get<std::string>();
            auto fragment_filepath = asset_info["fragmentFilepath"].get<std::string>();
            // TODO: Implement sound loading
            add<ShaderLoader>(name, m_base_dir / vertex_filepath, m_base_dir / fragment_filepath);
          }
          break;

        default:
          spdlog::warn("Unknown asset type {}", type_tag);
          break;
      }
    }
  }
}
}
