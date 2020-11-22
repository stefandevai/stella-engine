#include "stella/core/shader_loader.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/fileutils.hpp"

namespace stella
{
namespace core
{
  ShaderLoader::ShaderLoader (const std::string& vertex_filepath, const std::string& fragment_filepath)
    : AssetLoader(AssetType::SHADER), m_vertex_filepath(vertex_filepath), m_fragment_filepath(fragment_filepath)
  {

  }

  std::shared_ptr<Asset> ShaderLoader::construct()
  {
    const auto vertex_source = FileUtils::read_file(m_vertex_filepath);
    const auto fragment_source = FileUtils::read_file(m_fragment_filepath);
    return std::make_shared<graphics::ShaderProgram>(vertex_source.c_str(), fragment_source.c_str());
  }

}
}
