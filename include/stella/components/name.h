#pragma once

#include <string>

namespace stella
{
namespace components
{
  struct Name
  {
    Name (const std::string& name) : name (name) {}
    std::string name;
  };
} // namespace components
} // namespace stella
