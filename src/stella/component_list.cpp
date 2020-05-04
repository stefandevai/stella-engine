#include "stella/component_list.hpp"
#include "stella/components.hpp"
#include <iostream>

namespace stella
{
    void ComponentList::emplace_default_component (Components component_type, entt::entity entity, entt::registry& registry)
    {
        switch (component_type)
        {
        case AnimationPlayer:
            registry.emplace_or_replace<component::AnimationPlayer>(entity);
            break;
        case Body2D:
            registry.emplace_or_replace<component::Body2D>(entity);
            break;
        case Camera:
            registry.emplace_or_replace<component::Camera>(entity);
            break;
        case CharacterAnimation:
            registry.emplace_or_replace<component::CharacterAnimation>(entity);
            break;
        case Charcode:
            registry.emplace_or_replace<component::Charcode>(entity);
            break;
        case Color:
            registry.emplace_or_replace<component::Color>(entity);
            break;
        case Dimension:
            registry.emplace_or_replace<component::Dimension>(entity);
            break;
        case Layer:
            registry.emplace_or_replace<component::LayerT>(entity);
            break;
        case NPC:
            registry.emplace_or_replace<component::NPC>(entity);
            break;
        case Position:
            registry.emplace_or_replace<component::Position>(entity);
            break;
        case Script:
            registry.emplace_or_replace<component::Script>(entity);
            break;
        case Shape:
            registry.emplace_or_replace<component::Shape>(entity);
            break;
        case SpeechContainer:
            registry.emplace_or_replace<component::SpeechContainer>(entity);
            break;
        case SpriteGroup:
            registry.emplace_or_replace<component::SpriteGroup>(entity);
            break;
        case Sprite:
            registry.emplace_or_replace<component::SpriteT>(entity);
            break;
        case Text:
            registry.emplace_or_replace<component::Text>(entity);
            break;
        case Tile:
            registry.emplace_or_replace<component::Tile>(entity);
            break;
        case Timer:
            registry.emplace_or_replace<component::Timer>(entity);
            break;
        case Typewriter:
            registry.emplace_or_replace<component::Typewriter>(entity);
            break;
        default:
            std::cout << "ERROR: Trying to emplace unknown component.\n";
            break;
        }
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