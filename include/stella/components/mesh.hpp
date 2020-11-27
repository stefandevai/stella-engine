#pragma once

#include "component.hpp"
#include "stella/graphics/renderer/mesh.hpp"

namespace stella::component
{
  struct Mesh : public Component
  {
    Mesh (const std::string& resource_id) : Component ("Mesh"), resource_id (resource_id) {}
    std::string resource_id;
    std::shared_ptr<graphics::Mesh> mesh = nullptr;
  };
}

