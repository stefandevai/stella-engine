#include "worm.h"

bool MovementComponent::Finished = false;

Worm::Worm(entityx::EntityManager &entities) : entities(entities) {
  this->Velocity = 1;
  this->Dimension = 16;
  Tex =
      new stella::graphics::Texture("snake-tex", "assets/sprites/snake16.png");

  this->create(6);
}

Worm::~Worm() { delete Tex; }

void Worm::configure(entityx::EventManager &events) {
  events.subscribe<Collect>(*this);
}

void Worm::create(size_t size) {
	 //Create underhead to allow smooth movement
	under_head = entities.create();
	under_head.assign<TextureComponent>(this->Dimension, this->Dimension,
																			*Tex, 4);
	under_head.assign<SpatialComponent>(this->Dimension, this->Dimension, 320+Dimension*5,
																			304);
	under_head.assign<MovementComponent>(3, this->Velocity, false);

  // Create head
  this->head = entities.create();
  this->head.assign<TextureComponent>(this->Dimension, this->Dimension,
                                      *Tex, 4);
  this->head.assign<SpatialComponent>(this->Dimension, this->Dimension, 320+Dimension*5,
                                      304);
  this->head.assign<MovementComponent>(3, this->Velocity);
  this->head.assign<InputComponent>();
  this->head.assign<BodyComponent>(false);

  for (unsigned int i = 0; i < size; ++i) {
    this->AddBodyPart();
  }
}

void Worm::RemoveBodyPart() {
  assert(this->body.size() > 0);
  entityx::Entity &ent = this->body.back();
  ent.destroy();
  this->body.pop_back();

	entityx::Entity &bent = this->body.back();
	entityx::ComponentHandle<TextureComponent> tex =
			bent.component<TextureComponent>();
	entityx::ComponentHandle<MovementComponent> mov =
			bent.component<MovementComponent>();
	tex->sprite->SetDirectFrame(16);
	mov->Eased = true;
}

void Worm::AddBodyPart() {
  entityx::ComponentHandle<TextureComponent> tex;
  entityx::ComponentHandle<SpatialComponent> spa;
  entityx::ComponentHandle<MovementComponent> mov;
  entityx::Entity body_part = entities.create();

  // In case of existing tail
	if (this->body.size() > 0) {
    entityx::Entity &tail = this->body.back();
    mov = tail.component<MovementComponent>();
    spa = tail.component<SpatialComponent>();

    // Place tail in grid again
		//if (spa->x % this->Dimension != 0) {
			//int diff = this->Dimension - spa->x % this->Dimension;
			//if (mov->Direction == 3) spa->x += diff;
		//}
		//else if (spa->y % this->Dimension != 0) {
		//}
		//mov->Eased = false;
		tex = tail.component<TextureComponent>();
		tex->sprite->SetDirectFrame(4);
  }
  // If there's no tail yet
  else {
    spa = this->head.component<SpatialComponent>();
    mov = this->head.component<MovementComponent>();
  }

	// Add next body part according to the direction of movement
	switch (mov->Direction) {
	case 0:
		body_part.assign<SpatialComponent>(this->Dimension, this->Dimension, spa->x,
																			 spa->y + this->Dimension);
		break;
	case 1:
		body_part.assign<SpatialComponent>(this->Dimension, this->Dimension,
																			 spa->x - this->Dimension, spa->y);
		break;
	case 2:
		body_part.assign<SpatialComponent>(this->Dimension, this->Dimension, spa->x,
																			 spa->y - this->Dimension);
		break;
	case 3:
		body_part.assign<SpatialComponent>(this->Dimension, this->Dimension,
																			 spa->x + this->Dimension, spa->y);
		break;
	}
  body_part.assign<MovementComponent>(mov->Direction, this->Velocity);
  body_part.assign<TextureComponent>(this->Dimension, this->Dimension,
                                     *Tex, 14);
	this->body.push_back(body_part);
}

void Worm::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
  if (true) {
    entityx::ComponentHandle<MovementComponent> hmov =
        this->head.component<MovementComponent>();
		entityx::ComponentHandle<MovementComponent> uhmov =
				this->under_head.component<MovementComponent>();
		uhmov->NewDirection = hmov->Direction;
    
    if (hmov->Finished) {
      // Handle other bodyparts
      int lastDir = hmov->Direction;
      for (auto i = this->body.begin(); i != this->body.end(); ++i) {
        entityx::ComponentHandle<MovementComponent> mov =
            i->component<MovementComponent>();
        mov->NewDirection = lastDir;

        // If its not the hidden head
        //if (i != this->body.begin())
          lastDir = mov->Direction;
      }
    }
  } else {
    std::cout << "LOST!" << std::endl;
  }
}

void Worm::receive(const Collect &collectible) {
	this->AddBodyPart();
}
