#pragma once

#include "stella/components/position.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
    struct Position
    {
        void operator () (component::Position& pos)
        {
            float pos_input[3] = {0.0f, 0.0f, 0.0f};
            pos_input[0]       = pos.x;
            pos_input[1]       = pos.y;
            pos_input[2]       = pos.z;
            ImGui::PushID ("position#inspector");
            ImGui::DragFloat ("x", &pos_input[0], 1.0f, 0.0f, 0.0f, "%.3f");
            ImGui::DragFloat ("y", &pos_input[1], 1.0f, 0.0f, 0.0f, "%.3f");
            ImGui::DragFloat ("z", &pos_input[2], 1.0f, 0.0f, 0.0f, "%.3f");
            ImGui::PopID();
            pos.x = pos_input[0];
            pos.y = pos_input[1];
            pos.z = pos_input[2];
        }
    };
}
}