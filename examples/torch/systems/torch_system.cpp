#include "torch_system.h"

#include "../components/game_components.h"

TorchSystem::TorchSystem(entityx::Entity player) : Player(player) {

}

TorchSystem::~TorchSystem() {

}

void TorchSystem::configure(entityx::EventManager &events) {
  events.subscribe<Collision>(*this);
}

void TorchSystem::receive(const Collision &collision) {
  auto en = collision.Snowflake;
  en.destroy();
}

void TorchSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
  es.each<TorchComponent, PositionComponent>([this](entityx::Entity entity,
                                                      TorchComponent &torch,
                                                      PositionComponent &pos) {
      auto player_component = this->Player.component<PlayerComponent>();
      if (player_component->HasTorch) {
        auto player_pos = this->Player.component<PositionComponent>();
        pos.x = player_pos->x;
        pos.y = player_pos->y;
      }

  });
}



