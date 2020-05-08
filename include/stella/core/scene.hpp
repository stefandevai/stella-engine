#pragma once

#include <string>

namespace stella
{
namespace core
{

  class Scene
  {
    public:
      Scene(const std::string& name, const std::string& script_path);
      virtual ~Scene();

      virtual void load();
      virtual void update();
      virtual void render();

    protected:
      const std::string& m_name;
      const std::string& m_script_path;
  };

} // namespace 
} // namespace stella

