#include "worm.h"

bool MovementComponent::Finished = false;

Worm::Worm(entityx::EntityManager &entities, const std::array<bool, 1024> &keys)
    : entities(entities), keys(keys) {
  this->Velocity = 1;
  this->Dimension = 16;
  Tex =
      new stella::graphics::Texture("snake-tex", "assets/sprites/snake16.png");

  this->create(3);
}

Worm::~Worm() { delete Tex; }

void Worm::configure(entityx::EventManager &events) {
  events.subscribe<Collect>(*this);
}

void Worm::create(size_t size) {
  // Create underhead to allow smooth movement
  auto under_head = entities.create();
  under_head.assign<TextureComponent>(this->Dimension, this->Dimension,
                                      *Tex, 4);
  under_head.assign<SpatialComponent>(this->Dimension, this->Dimension, 320,
                                      304);
  under_head.assign<MovementComponent>(3, this->Velocity, false);
  this->body.push_back(under_head);

  // Create head
  this->head = entities.create();
  this->head.assign<TextureComponent>(this->Dimension, this->Dimension,
                                      *Tex, 4);
  this->head.assign<SpatialComponent>(this->Dimension, this->Dimension, 320,
                                      304);
  this->head.assign<MovementComponent>(3, this->Velocity);
  this->head.assign<InputComponent>(keys);
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
  tex->sprite->SetDirectFrame(4);
  mov->Eased = true;
}

void Worm::AddBodyPart() {
  entityx::ComponentHandle<TextureComponent> tex;
  entityx::ComponentHandle<SpatialComponent> spa;
  entityx::ComponentHandle<MovementComponent> mov;

  // In case of existing tail
  if (this->body.size() > 0) {
    entityx::Entity &bent = this->body.back();
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
                                     *Tex, 12);
  this->body.push_back(body_part);
}

void Worm::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
  if (this->body.size() > 0) {
    entityx::ComponentHandle<MovementComponent> hmov =
        this->head.component<MovementComponent>();
    if (hmov->Finished) {
      // Handle other bodyparts
      int lastDir = hmov->Direction;
      for (auto i = this->body.begin(); i != this->body.end(); ++i) {
        entityx::ComponentHandle<MovementComponent> mov =
            i->component<MovementComponent>();
        mov->NewDirection = lastDir;

        // If its not the hidden head
        if (i != this->body.begin())
          lastDir = mov->Direction;
      }
    }
  } else {
    std::cout << "LOST!" << std::endl;
  }
}

void Worm::receive(const Collect &collectible) {
	this->AddBodyPart();
	std::cout << "Collected!" << std::endl;
}
