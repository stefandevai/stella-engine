#include "editor/project.hpp"

namespace stella
{
namespace editor
{
  Project::Project(const std::string& name, const unsigned int width, const unsigned int height)
    : m_name(name), m_width(width), m_height(height)
  {

  }

  Project::Project(const std::string& filepath)
    : m_filepath(filepath)
  {

  }

  void Project::save (const std::string& filepath)
  {

  }

  void Project::load (const std::string& filepath)
  {

  }

  void Project::close()
  {

  }
}
}
