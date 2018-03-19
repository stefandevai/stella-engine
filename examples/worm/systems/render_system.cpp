#include "render_system.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/spatial_component.h"
#include "../components/sprite_component.h"

RenderSystem::RenderSystem(int width, int height, stella::graphics::Shader *shad) {
	this->shader = shad;
	glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	this->TileLayer = new SceneLayer(shad, proj);
}

RenderSystem::~RenderSystem() {
	delete this->TileLayer;
}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<SpatialComponent, TextureComponent>([this](entityx::Entity entity, SpatialComponent &spa, TextureComponent &tex) {
		tex.sprite->Pos.x = spa.x;
		tex.sprite->Pos.y = spa.y;
		if (spa.x == 16)
			std::cout << "here" << std::endl;

		if (!tex.InLayer) {
			this->TileLayer->Add(tex.sprite);
			tex.InLayer = true;
		}
		this->TileLayer->Render();
	});
};


void RenderSystem::configure(entityx::EventManager &event_manager) {
	event_manager.subscribe<entityx::ComponentRemovedEvent<TextureComponent>>(*this);
}

void RenderSystem::receive(const entityx::ComponentRemovedEvent<TextureComponent> &ev) {
	auto ent = ev.entity;
	auto tex = ent.component<TextureComponent>();
	if (tex->InLayer) {
		this->TileLayer->Remove(tex->sprite);
		tex->InLayer = false;
	}
}

