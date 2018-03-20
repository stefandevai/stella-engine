#include "collect_system.h"
#include "../events/collect.h"

CollectSystem::CollectSystem() {
	this->Collectible = CollectibleType::None;
}

CollectSystem::~CollectSystem() {}

void CollectSystem::configure(entityx::EventManager &events) {
  events.subscribe<Collision>(*this);
}

void CollectSystem::update(entityx::EntityManager &entities,
                           entityx::EventManager &events,
													 entityx::TimeDelta dt) {
	if (this->Collectible != CollectibleType::None) {
		events.emit<Collect>(this->Collectible);
		this->Collectible = CollectibleType::None;
	}
}

void CollectSystem::receive(const Collision &collision) {
  auto en = collision.Static;
  auto collectible = en.component<CollectibleComponent>();
  this->Collectible = collectible->Type;
  en.destroy();
}
