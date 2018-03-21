#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "scenelayer.h"
#include "../components/sprite_component.h"

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem> {
public:
  RenderSystem(int width, int height);
  ~RenderSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
  void configure(entityx::EventManager &event_manager);
  void receive(const entityx::ComponentRemovedEvent<TextureComponent> &ev);

private:
  SceneLayer *TileLayer;
  stella::graphics::Shader *Shader;
};
