#include "torch_system.h"

#include "../components/game_components.h"

TorchSystem::TorchSystem(entityx::Entity player) : Player(player) {

}

TorchSystem::~TorchSystem() {

}

void TorchSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
  es.each<TorchComponent, SpatialComponent>([this](entityx::Entity entity,
                                                      TorchComponent &torch,
                                                      SpatialComponent &spa) {
      //std::cout << "here" << std::endl;
      auto player_component = this->Player.component<PlayerComponent>();
      if (player_component->HasTorch) {
        auto player_spa = this->Player.component<SpatialComponent>();
        spa.x = player_spa->x;
        spa.y = player_spa->y;
      }

  });
}



