#pragma once

#include <unordered_map>
#include <string>

namespace stella
{
  enum class SystemTag
  {
    NONE,
    RENDER,
    ANIMATION,
  };

  typedef std::unordered_map<std::string, SystemTag> SystemMap;

  const SystemMap g_system_map =
  {
    { "render", SystemTag::RENDER },
    { "animation", SystemTag::ANIMATION },
  };
}
