#pragma once

#include <string>

namespace stella
{
namespace component
{
  struct Name
  {
    Name (const std::string& name) : name (name) {}
    std::string name;
  };
} // namespace component
} // namespace stella
