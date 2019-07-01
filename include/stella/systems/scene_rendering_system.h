#pragma once

#include <unordered_map>
#include <entityx/entityx.h>

#include "stella/graphics/scenelayer.h"
#include "stella/graphics/firelayer.h"
#include "stella/graphics/layers/basic_layer.h"

#include "../components.h"

namespace ex = entityx;

namespace stella {
namespace graphics {
  class Display;
  class Texture;
}
}

namespace stella {
namespace systems {
class SceneRenderingSystem : public ex::System<SceneRenderingSystem>, public ex::Receiver<SceneRenderingSystem> {
public:
  SceneRenderingSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &textures, stella::graphics::Display& display);
  ~SceneRenderingSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
  void configure(ex::EventManager &event_manager) override;
  void receive(const ex::ComponentRemovedEvent<components::SpriteComponent> &ev);

private:
  std::vector<graphics::BasicLayer> layers;
  graphics::SceneLayer *TileLayer;
  graphics::FireLayer *ParticleLayer;
  stella::graphics::Shader *Shader;
  std::unordered_map<std::string, stella::graphics::Texture*> &Textures;
};
} // namespace systems
} // namespace stella
