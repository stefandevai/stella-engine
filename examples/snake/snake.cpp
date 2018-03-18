#include "snake.h"

Snake::Snake(entityx::EntityManager &entities, const bool keys[1024]) : entities(entities), keys(keys){
	this->create();
}

Snake::~Snake() {
	delete SnakeTex;
}

void Snake::create() {
	SnakeTex = new stella::graphics::Texture("snake-tex", "assets/gfx/snake/snake.png");
	this->head = entities.create();

	this->head.assign<TextureComponent>(32, 32, *SnakeTex, 0);
	this->head.assign<SpatialComponent>(32, 32, 320, 320);
	this->head.assign<MovementComponent>(3, 40);
	this->head.assign<InputComponent>(keys);

	entityx::Entity tail = entities.create();
	tail.assign<TextureComponent>(32, 32, *SnakeTex, 2);
	tail.assign<SpatialComponent>(32, 32, 352 + 32*4, 320);
	tail.assign<MovementComponent>(3, 40);
	this->body.push_back(tail);
	
	for (int i = 0; i < 4; ++i) {
		entityx::Entity body_part = entities.create();
		body_part.assign<TextureComponent>(32, 32, *SnakeTex, 1);
		body_part.assign<SpatialComponent>(32, 32, 352 + 32*i, 320);
		body_part.assign<MovementComponent>(3, 40);
		this->body.push_back(body_part);
	}

	entityx::Entity& ent = this->body.back();
	entityx::ComponentHandle<SpatialComponent> spab = ent.component<SpatialComponent>();
	entityx::Entity& tail2 = this->body.front();
	entityx::ComponentHandle<SpatialComponent> spat = tail2.component<SpatialComponent>();
	spat->x = spab->x;
	spat->y = spab->y;

	ent.destroy();
	this->body.pop_back();
}

