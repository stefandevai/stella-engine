#include "stella/components/component.hpp"
#include <spdlog/spdlog.h>

namespace stella::component
{
Component::Component (const std::string& name) : name (name)
{
  try
  {
    type = COMPONENT_MAP.at (name);
  }
  catch (std::out_of_range& e)
  {
    spdlog::critical ("Component tag not found: {}\n{}", name, e.what());
    type = ComponentType::NONE;
  }
}

Component::~Component() {}
} // namespace stella::component
