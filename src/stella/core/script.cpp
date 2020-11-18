#include "stella/core/script.hpp"

namespace stella
{
namespace core
{
  Script::Script()
  {
    lua.open_libraries (sol::lib::base);
  }

  Script::Script(const std::string& script_path)
  {
    lua.open_libraries (sol::lib::base);
    load(script_path);
  }

  void Script::load(const std::string& script_path)
  {
    lua.script_file(script_path);
  }
}
}
