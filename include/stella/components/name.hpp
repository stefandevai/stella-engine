#pragma once

#include "component.hpp"

#include <string>

namespace stella
{
namespace component
{
  struct Name : public Component
  {
    Name (const std::string& name) : Component ("Name"), name (name) {}
    std::string name;
  };
} // namespace component
} // namespace stella
