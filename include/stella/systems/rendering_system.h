#pragma once

#include <unordered_map>
#include <map>
#include <entityx/entityx.h>

#include "stella/graphics/scenelayer.h"
#include "stella/graphics/firelayer.h"
#include "stella/graphics/layers/basic_layer.h"

#include "../components/game_components.h"

#define MAX_NUM_LAYERS 25

namespace ex = entityx;

namespace stella {
namespace graphics {
  class Display;
  class Texture;
}
}

namespace stella {
namespace systems {
class RenderingSystem : public ex::System<RenderingSystem>, public ex::Receiver<RenderingSystem> {
public:
  RenderingSystem(std::unordered_map<std::string, stella::graphics::Texture*> &textures, stella::graphics::Display& display);
  ~RenderingSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
  void configure(ex::EventManager &event_manager) override;
  void receive(const ex::ComponentRemovedEvent<components::SpriteComponent> &ev);

private:
  std::unordered_map<std::string, std::shared_ptr<graphics::BasicLayer>> layers;
  std::unordered_map<std::string, int> layer_order;
  std::unordered_map<std::string, stella::graphics::Texture*> &Textures;
  const graphics::Display &Display;
};
} // namespace systems
} // namespace stella

