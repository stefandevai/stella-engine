#pragma once

#include "stella/components/tile.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
    struct Tile
    {
        void operator () (component::Tile& tile)
        {
            int layer_id    = static_cast<int> (tile.layer_id);
            bool collidable = tile.collidable;
            ImGui::PushID ("tile#inspector");
            ImGui::InputInt ("layer_id", &layer_id);
            ImGui::Checkbox ("collidable", &collidable);
            ImGui::PopID();
            tile.layer_id   = static_cast<unsigned> (layer_id);
            tile.collidable = collidable;
        }
    };
}
}