#pragma once

#include "stella/components/scroll.hpp"
#include "../components.hpp"
#include "./system.hpp"

namespace stella
{
namespace system
{
  class TiledScroll : public System
  {
  private:
    const int BoundX;

  public:
    TiledScroll (const int& boundx) : BoundX (boundx) {}

    ~TiledScroll() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.group<component::Tileview> (entt::get<component::Position, component::Dimension>)
          .each ([this] (auto entity, auto& pos, auto& dim) {
            if (pos.x + dim.w < 0)
            {
              pos.x = this->BoundX + (pos.x + dim.w);
            }
          });
    }
  };
} // namespace system
} // namespace stella
