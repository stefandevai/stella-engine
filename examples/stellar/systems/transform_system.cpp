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

    if ((int)(spr.Sprite->RealDimensions.x) != spa.w) {
      trans.Scale.x *= static_cast<float>(spa.w/spr.Sprite->Dimensions.x);
      spr.Sprite->RealDimensions.x = spa.w;
    }
    if ((int)(spr.Sprite->RealDimensions.y) != spa.h) {
      trans.Scale.y *= static_cast<double>(spa.h/spr.Sprite->Dimensions.y);
      spr.Sprite->RealDimensions.y = spa.h;
    }
    spr.Sprite->SetScale(trans.Scale);
    spr.Sprite->SetRotation(trans.Rotation);
  });
}


