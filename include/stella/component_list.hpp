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
    Layer,
    NPC,
    Position,
    Script,
    Shape,
    SpeechContainer,
    SpriteGroup,
    Sprite,
    Text,
    Tile,
    Timer,
    Typewriter
};

struct ComponentList
{

static const std::map<Components, std::string> component_map;

    static void emplace_default_component (Components component_type, entt::entity entity, entt::registry& registry);
};
}