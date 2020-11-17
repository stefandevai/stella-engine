#include "stella/tilemap/tilemap.hpp"

namespace stella
{
namespace tilemap
{
  Tilemap::Tilemap (const std::string& name, const std::string& script_path) : m_name (name), m_script_path (script_path) {}

  Tilemap::~Tilemap() {}

  void Tilemap::load() {}

  void Tilemap::update() {}

  void Tilemap::render() {}

} // namespace tilemap
} // namespace stella

