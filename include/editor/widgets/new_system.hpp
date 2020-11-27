#pragma once

#include "widget.hpp"

namespace stella
{
namespace core
{
  class Scene;
}
} // namespace stella

namespace editor
{
namespace widget
{
  struct NewSystem : public Widget
  {
  public:
    NewSystem();
    std::string render (const std::shared_ptr<stella::core::Scene>& scene);
  };
} // namespace widget
} // namespace editor
