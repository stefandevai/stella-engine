#include "snake.h"

bool MovementComponent::Finished = false;

Snake::Snake(entityx::EntityManager &entities, const bool keys[1024]) : entities(entities), keys(keys){
	this->Velocity = 1;
	this->Dimension = 16;
	SnakeTex = new stella::graphics::Texture("snake-tex", "assets/sprites/snake16.png");

	this->create(3);
}

Snake::~Snake() {
	delete SnakeTex;
}

void Snake::create(size_t size) {
	// Create underhead to allow smooth movement
	auto under_head = entities.create();
	under_head.assign<TextureComponent>(this->Dimension, this->Dimension, *SnakeTex, 4);
	under_head.assign<SpatialComponent>(this->Dimension, this->Dimension, 320, 304);
	under_head.assign<MovementComponent>(3, this->Velocity, false);
	this->body.push_back(under_head);

	// Create head
	this->head = entities.create();
	this->head.assign<TextureComponent>(this->Dimension, this->Dimension, *SnakeTex, 4);
	this->head.assign<SpatialComponent>(this->Dimension, this->Dimension, 320, 304);
	this->head.assign<MovementComponent>(3, this->Velocity);
	this->head.assign<InputComponent>(keys);
	this->head.assign<BodyComponent>(false);

	for (unsigned int i = 0; i < size; ++i) {
		this->AddBodyPart();
	}
}

void Snake::RemoveBodyPart() {
	assert(this->body.size() > 0);
	entityx::Entity &ent = this->body.back();
	ent.destroy();
	this->body.pop_back();

	entityx::Entity& bent = this->body.back();
	entityx::ComponentHandle<TextureComponent> tex = bent.component<TextureComponent>();
	entityx::ComponentHandle<MovementComponent> mov = bent.component<MovementComponent>();
	tex->sprite->SetDirectFrame(4);
	mov->Eased = true;
}

void Snake::AddBodyPart() {
	entityx::ComponentHandle<TextureComponent> tex;
	entityx::ComponentHandle<SpatialComponent> spa;
	entityx::ComponentHandle<MovementComponent> mov;

	// In case of existing tail
	if (this->body.size() > 0) {
		entityx::Entity& bent = this->body.back();
		tex = bent.component<TextureComponent>();
		spa = bent.component<SpatialComponent>();
		mov = bent.component<MovementComponent>();

		mov->Eased = false;
		tex->sprite->SetDirectFrame(4);
	}
	// If there's no tail yet
	else {
		spa = this->head.component<SpatialComponent>();
		mov = this->head.component<MovementComponent>();
	}

	// Add next body part according to the direction of movement
	entityx::Entity body_part = entities.create();
	switch(mov->Direction) {
		case 0:
			body_part.assign<SpatialComponent>(this->Dimension, this->Dimension, spa->x, spa->y + this->Dimension);
			break;
		case 1:
			body_part.assign<SpatialComponent>(this->Dimension, this->Dimension, spa->x - this->Dimension, spa->y);
			break;
		case 2:
			body_part.assign<SpatialComponent>(this->Dimension, this->Dimension, spa->x, spa->y - this->Dimension);
			break;
		case 3:
			body_part.assign<SpatialComponent>(this->Dimension, this->Dimension, spa->x + this->Dimension, spa->y);
			break;
	}

	body_part.assign<MovementComponent>(mov->Direction, this->Velocity);
	body_part.assign<TextureComponent>(this->Dimension, this->Dimension, *SnakeTex, 12);
	this->body.push_back(body_part);
}

void Snake::Update() {
	if (this->body.size() > 0) {
		entityx::ComponentHandle<MovementComponent> hmov = this->head.component<MovementComponent>();
		if (hmov->Finished) {
		  // Handle other bodyparts
			int lastDir = hmov->Direction;
			for (auto i = this->body.begin(); i != this->body.end(); ++i) {
				entityx::ComponentHandle<MovementComponent> mov = i->component<MovementComponent>();
				mov->NewDirection = lastDir;

				// If its not the hidden head
				if (i != this->body.begin())
					lastDir = mov->Direction;
			}
		}
	}	
	else {
		std::cout << "LOST!" << std::endl;
	}
}

