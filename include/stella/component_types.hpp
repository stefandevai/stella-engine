#pragma once

#include <unordered_map>
#include <string>

namespace stella
{
enum class ComponentType
{
  NONE,
  SPRITE,
  MESH,
};

std::unordered_map<std::string, ComponentType> COMPONENT_MAP{
    {"sprite", ComponentType::SPRITE},
    {"mesh", ComponentType::MESH},
};

} // namespace stella
