#include <sstream>

#include "stella/components.h"
#include "stella/systems/tiles_system.h"

namespace stella {
namespace systems {
TilesSystem::TilesSystem(ex::Entity &camera) : Camera(camera) { }
TilesSystem::~TilesSystem() { }

void TilesSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
  const auto &camerapos = this->Camera.component<components::PositionComponent>();
  es.each<components::TileComponent, components::PositionComponent, components::DimensionComponent>([camerapos](ex::Entity entity,
                                                      components::TileComponent &tile,
                                                      components::PositionComponent &pos,
                                                      components::DimensionComponent &dim) {
      if (pos.x + dim.w < camerapos->x - dim.w)
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

