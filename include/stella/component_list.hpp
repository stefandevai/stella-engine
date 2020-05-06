#pragma once

#include <map>
#include <string>
#include <entt/entity/registry.hpp>
#include <entt/entity/entity.hpp>

namespace stella
{
enum Components
{
  AnimationPlayer,
  Body2D,
  Camera,
  CharacterAnimation,
  Charcode,
  Color,
  Dimension,
  Group,
  Layer,
  NPC,
  Position,
  Script,
  Shape,
  SpeechContainer,
  Sprite,
  Text,
  Tile,
  Timer,
  Typewriter,
  Vertical
};

struct ComponentList
{
  static const std::map<Components, std::string> component_map;

  static void emplace_default_component (Components component_type, entt::entity entity, entt::registry& registry);
};
} // namespace stella