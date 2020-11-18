#pragma once

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp> // IWYU pragma: export

namespace stella
{
namespace core
{
  class Script
  {
  public:
    sol::state lua;

  public:
    Script();
    Script(const std::string& script_path);

    void load(const std::string& script_path);

  private:
    std::string m_script_path;
  };
}
}
