#include "transform_system.h"

#include "../components/transform_component.h"
#include "../components/spatial_component.h"
#include "../components/sprite_component.h"

TransformSystem::TransformSystem() {

}

TransformSystem::~TransformSystem() {

}

void TransformSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
  es.each<TransformComponent, SpatialComponent, SpriteComponent>([this](entityx::Entity entity,
                                                      TransformComponent &trans,
                                                      SpatialComponent &spa,
                                                      SpriteComponent &spr) {

    spr.Sprite->SetScale(trans.Scale);
    spr.Sprite->SetRotation(trans.Rotation);
  });
}


