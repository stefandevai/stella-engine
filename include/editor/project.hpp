#pragma once

#include <string>

namespace stella
{
namespace editor
{
  class Project
  {
  public:
    Project (const std::string& name, const unsigned int width, const unsigned int height);
    Project (const std::string& filepath);

    void save (const std::string& filepath);
    void load (const std::string& filepath);
    void close();

  private:
    std::string m_name, m_filepath;
    int m_width = 0;
    int m_height = 0;
  };
}
}

