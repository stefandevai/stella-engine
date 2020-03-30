#include "stella/graphics/layers/water_layer.hpp"

#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shape_renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace stella
{
namespace graphics
{
  WaterLayer::WaterLayer (float width, float height, bool fixed)
    : ShapeLayer (std::make_shared<graphics::ShapeRenderer>(), fixed)
  {
    this->Shad =
        std::make_shared<graphics::Shader> ("assets/shaders/water_shader.vert", "assets/shaders/water_shader.frag");
    this->Shad->Enable();
    const auto projection = glm::ortho (0.0f, width, height, 0.0f, -20.0f, 10.0f);
    this->Shad->SetMat4 ("proj", projection);
    this->Shad->Disable();
  }
  WaterLayer::~WaterLayer() {}

  void WaterLayer::Render()
  {
    this->Shad->Enable();
    this->Ren->Begin();

    for (auto& i : Shapes)
      this->Ren->Submit (*i);

    this->Ren->End();
    this->Ren->Draw();
  }
} // namespace graphics
} // namespace stella
