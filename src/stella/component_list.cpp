#include "stella/component_list.hpp"

namespace stella
{
    void ComponentList::emplace_default_component (Components component_type, entt::entity entity, entt::registry& registry)
    {
        
    }

    const std::map<Components, std::string> ComponentList::component_map = {
    { AnimationPlayer, "Animation Player" },
    { Body2D, "Body2D" },
    { Camera, "Camera" },
    { CharacterAnimation , "Character Animation" },
    { Charcode, "Charcode" },
    { Color, "Color" },
    { Dimension, "Dimension" },
    { Layer, "Layer" },
    { Log, "Log" },
    { NPC, "NPC" },
    { Position, "Position" },
    { Script, "Script" },
    { Shape, "Shape" },
    { SpeechContainer, "Speech Container" },
    { SpriteGroup, "Sprite Group" },
    { Sprite, "Sprite" },
    { Text, "Text" },
    { Tile, "Tile" },
    { Timer, "Timer" },
    { Typewriter, "Typewriter" }
};
}