#pragma once

#include "stella/component_types.hpp"
#include <string>

namespace stella::component
{
struct Component
{
  Component (const std::string& name);
  virtual ~Component();
  std::string name;
  bool loaded      = false;
  ComponentType type = ComponentType::NONE;
};
} // namespace stella
