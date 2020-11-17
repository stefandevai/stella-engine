#pragma once

#include <string>

namespace stella
{
namespace tilemap
{
  class Tilemap
  {
  public:
    Tilemap (const std::string& name, const std::string& script_path);
    virtual ~Tilemap();

    virtual void load();
    virtual void update();
    virtual void render();

  protected:
    const std::string& m_name;
    const std::string& m_script_path;
  };

} // namespace core
} // namespace stella

