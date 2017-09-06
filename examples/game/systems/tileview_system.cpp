#include "tileview_system.h"

#include "../components/tileview_component.h"
#include "../components/spatial_component.h"

TileviewSystem::TileviewSystem(const int &boundx) : BoundX(boundx) {
}

TileviewSystem::~TileviewSystem() {
}

void TileviewSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<TileviewComponent, SpatialComponent>([this](entityx::Entity entity, TileviewComponent &til, SpatialComponent &spa) {
			if (spa.x + spa.w < 0) spa.x = BoundX;
	});
}

