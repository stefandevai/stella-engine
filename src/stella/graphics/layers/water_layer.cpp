#include "stella/graphics/layers/water_layer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stella/graphics/opengl.h"
#include "stella/graphics/shape_renderer.h"

namespace stella {
namespace graphics {
WaterLayer::WaterLayer(float width, float height, bool fixed)
  : ShapeLayer(std::make_shared<graphics::ShapeRenderer>(), fixed) {
    this->Shad = std::make_shared<graphics::Shader>("assets/shaders/water_shader.vert", "assets/shaders/water_shader.frag");
    this->Shad->Enable();
    const auto projection = glm::ortho(0.0f, width, height, 0.0f, -20.0f, 10.0f);
    this->Shad->SetMat4("proj", projection);
    this->Shad->Disable();
}
WaterLayer::~WaterLayer() {}

void WaterLayer::Render() {
  this->Shad->Enable();
  this->Ren->Begin();

  for (auto &i : Shapes)
    this->Ren->Submit(*i);

  this->Ren->End();
  this->Ren->Draw();
}
} // namespace graphics
} // namespace stella


