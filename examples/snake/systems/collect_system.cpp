#include "collect_system.h"

CollectSystem::CollectSystem() {

}

CollectSystem::~CollectSystem() {

}

void CollectSystem::configure(entityx::EventManager &event_manager) {
	event_manager.subscribe<Collision>(*this);
}

void CollectSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) {}

void CollectSystem::receive(const Collision &collision) {
	auto en = collision.Static;
	en.destroy();
}

