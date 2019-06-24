#include "torch_system.h"

#include <sstream>

#include <components/game_components.h>

TorchSystem::TorchSystem(ex::Entity player, ex::EntityManager &es) : Player(player) {
  this->PointsString = es.create();
  this->PointsString.assign<PositionComponent>(30.f, 105.f);
  this->PointsString.assign<DimensionComponent>(9.f, 9.f);
	this->PointsString.assign<TextComponent>("Points: ", "font-cursive");
}

TorchSystem::~TorchSystem() {

}

void TorchSystem::configure(ex::EventManager &events) {
  events.subscribe<Collision>(*this);
}

void TorchSystem::receive(const Collision &collision) {
  auto en = collision.Snowflake;
  this->Points += 10;
  std::stringstream ss;
  ss << "Points:  " << this->Points;
  auto pstring = this->PointsString.component<TextComponent>();
  pstring->Text = ss.str();
  en.destroy();
}

void TorchSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
  es.each<TorchComponent, PositionComponent>([this](ex::Entity entity,
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



