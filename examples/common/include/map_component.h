#pragma once

#include <anax/Component.hpp>
#include <vector>

struct SpriteComponent;

struct MapComponent : anax::Component {
  std::vector<std::vector<SpriteComponent>> sprites;
}
