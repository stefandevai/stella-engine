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
  POSITION,
  COLOR,
  ANIMATION_PLAYER,
};

const std::unordered_map<std::string, ComponentType> COMPONENT_MAP
{
    {"Sprite", ComponentType::SPRITE},
    {"Mesh", ComponentType::MESH},
    {"AnimationPlayer", ComponentType::ANIMATION_PLAYER},
    {"Color", ComponentType::COLOR},
    {"Position", ComponentType::POSITION},
};

} // namespace stella
