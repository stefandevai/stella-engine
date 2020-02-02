#pragma once

#include "../components.h"
#include "./system.h"

namespace stella
{
namespace systems
{
  class TiledScrollSystem : public System
  {
  private:
    const int BoundX;

  public:
    TiledScrollSystem (const int& boundx) : BoundX (boundx) {}

    ~TiledScrollSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry
          .group<components::TileviewComponent> (
              entt::get<components::Position, components::Dimension>)
          .each ([this] (auto entity, auto& pos, auto& dim) {
            if (pos.x + dim.w < 0)
            {
              pos.x = this->BoundX + (pos.x + dim.w);
            }
          });
    }
  };
} // namespace systems
} // namespace stella
