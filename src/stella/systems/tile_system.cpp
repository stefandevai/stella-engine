#include <sstream>

#include "stella/components.h"
#include "stella/systems/tiles_system.h"

namespace stella {
namespace systems {
TilesSystem::TilesSystem() { }
TilesSystem::~TilesSystem() { }

void TilesSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
  es.each<components::TileComponent, components::PositionComponent, components::DimensionComponent>([this](ex::Entity entity,
                                                      components::TileComponent &tile,
                                                      components::PositionComponent &pos,
                                                      components::DimensionComponent &dim) {
      if (pos.x + dim.w < -dim.w)
      {
        if (entity.valid())
        {
          entity.destroy();
        }
      }
  });
}
} // namespace systems
} // namespace stella

