#pragma once

#include <string>

namespace stella
{
namespace component
{
  struct Component
  {
    Component (const std::string& name) : name (name) {}
    virtual ~Component() {}
    std::string name = "";
    bool loaded      = false;
  };
} // namespace component
} // namespace stella
