#include <sstream>

#include "stella/components/game_components.h"
#include "stella/systems/torch_system.h"

namespace stella {
namespace systems {
TorchSystem::TorchSystem(ex::Entity player, ex::EntityManager &es) : Player(player), PointsString(es.create()) {
  this->PointsString.assign<components::PositionComponent>(30.f, 105.f);
  this->PointsString.assign<components::DimensionComponent>(9.f, 9.f);
	this->PointsString.assign<components::TextComponent>("Points: ", "font-cursive");
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
  auto pstring = this->PointsString.component<components::TextComponent>();
  pstring->Text = ss.str();
  en.destroy();
}

void TorchSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
  es.each<components::TorchComponent, components::PositionComponent>([this](ex::Entity entity,
                                                      components::TorchComponent &torch,
                                                      components::PositionComponent &pos) {
      auto player_component = this->Player.component<components::PlayerComponent>();
      if (player_component->HasTorch) {
        auto player_pos = this->Player.component<components::PositionComponent>();
        pos.x = player_pos->x;
        pos.y = player_pos->y;
      }

  });
}
} // namespace systems
} // namespace stella

