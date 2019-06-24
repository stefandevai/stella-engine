#include "tileview_system.h"

#include <components/game_components.h>

TileviewSystem::TileviewSystem(const int &boundx) : BoundX(boundx) {}

TileviewSystem::~TileviewSystem() {}

void TileviewSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  es.each<TileviewComponent, PositionComponent, DimensionComponent>([this](entityx::Entity entity,
                                                      TileviewComponent &til,
                                                      PositionComponent &pos,
                                                      DimensionComponent &dim) {
    if (pos.x + dim.w < 0) {
      pos.x = this->BoundX + (pos.x + dim.w);
    }
  });
}
