#pragma once

#include "stella/components/camera.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
    struct Camera
    {
        void operator () (component::Camera& camera)
        {

        }
    };
}
}