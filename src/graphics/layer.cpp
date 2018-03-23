#include "stella/graphics/layer.h"

#include <algorithm>

namespace stella {
namespace graphics {
Layer::Layer(Renderer *renderer, Shader *shader, glm::mat4 projection)
    : Ren(renderer), Shad(shader), Projection(projection) {
  this->Shad->Enable();
  this->Shad->SetMat4("proj", projection);
  this->Shad->Disable();
}

Layer::~Layer() {
  this->Shad->Disable();
  for (auto i : this->Sprites)
    delete i;
  delete Ren;
}

void Layer::Add(Sprite *sprite) { this->Sprites.push_back(sprite); }

void Layer::Remove(Sprite *sprite) {
  auto it = std::find(this->Sprites.begin(), this->Sprites.end(), sprite);
  this->Sprites.erase(it);
}

void Layer::Render() {
	 this->Shad->Enable();
  this->Ren->Begin();
  for (auto i : Sprites)
    this->Ren->Submit(*i);
  this->Ren->End();
  this->Ren->Draw();
	 this->Shad->Disable();
}
} // namespace graphics
} // namespace stella
