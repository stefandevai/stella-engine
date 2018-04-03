#pragma once

#include <unordered_map>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "scenelayer.h"
#include "firelayer.h"

#include "../components/sprite_component.h"

namespace ex = entityx;

class RenderSystem : public ex::System<RenderSystem>, public ex::Receiver<RenderSystem> {
public:
  RenderSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &textures, stella::graphics::Display& display);
  ~RenderSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
  void configure(ex::EventManager &event_manager);
  void receive(const ex::ComponentRemovedEvent<SpriteComponent> &ev);

private:
  SceneLayer *TileLayer;
  FireLayer *ParticleLayer;
  stella::graphics::Shader *Shader;
  std::unordered_map<std::string, stella::graphics::Texture*> &Textures;
};
