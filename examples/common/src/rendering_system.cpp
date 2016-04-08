#include "rendering_system.h"

#include "position_component.h"
#include "sprite_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

RenderingSystem::RenderingSystem(int width, int height, stella::graphics::Shader *shad)
{
  this->shader = shad;
  glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  TileLayer = new SceneLayer(this->shader, proj);
}

RenderingSystem::~RenderingSystem()
{
  delete TileLayer;
}

void RenderingSystem::Prepare()
{
  auto entities = this->getEntities();
  for (auto& entity : entities)
  {
    auto& sprite = entity.getComponent<SpriteComponent>().sprite;
    auto& position = entity.getComponent<PositionComponent>();
    sprite->Pos.x = position.x;
    sprite->Pos.y = position.y;
    std::cout << sprite->Pos.x << std::endl;
    TileLayer->Add(sprite);
  }
}

void RenderingSystem::Render()
{
  this->TileLayer->Render();
}

