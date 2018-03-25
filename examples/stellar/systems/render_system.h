#pragma once

#include <unordered_map>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "scenelayer.h"
#include "firelayer.h"

#include "../components/sprite_component.h"

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem> {
public:
  RenderSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &textures, stella::graphics::Display& display);
  ~RenderSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
  void configure(entityx::EventManager &event_manager);
  void receive(const entityx::ComponentRemovedEvent<SpriteComponent> &ev);

private:
  SceneLayer *TileLayer;
  FireLayer *ParticleLayer;
  stella::graphics::Shader *Shader;
  std::unordered_map<std::string, stella::graphics::Texture*> &Textures;
};
