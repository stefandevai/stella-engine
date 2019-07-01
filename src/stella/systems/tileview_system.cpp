#include "stella/components.h"
#include "stella/systems/tileview_system.h"

namespace stella {
namespace systems {
TileviewSystem::TileviewSystem(const int &boundx) : BoundX(boundx) {}

TileviewSystem::~TileviewSystem() {}

void TileviewSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  es.each<components::TileviewComponent, components::PositionComponent, components::DimensionComponent>([this](entityx::Entity entity,
                                                      components::TileviewComponent &til,
                                                      components::PositionComponent &pos,
                                                      components::DimensionComponent &dim) {
    if (pos.x + dim.w < 0) {
      pos.x = this->BoundX + (pos.x + dim.w);
    }
  });
}
} // namespace systems
} // namespace stella
