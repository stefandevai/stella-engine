#pragma once

#include "component.hpp"

#include <string>

namespace stella
{
namespace component
{
  enum ScriptType
  {
    Once,
    Forever
  };

  enum ScriptState
  {
    Waiting,
    Active,
    Finished
  };

  struct ScriptComponent : public Component
  {
    ScriptComponent (const std::string& path, ScriptType type = Once) : Component ("Script"), path (path), type (type) {}
    std::string path;
    ScriptType type;
    ScriptState state = Waiting;
  };
} // namespace component
} // namespace stella
