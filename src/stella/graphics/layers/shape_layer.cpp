#include "stella/graphics/layers/shape_layer.h"
#include "stella/graphics/opengl.h"

#include <algorithm>

namespace stella
{
namespace graphics
{
  ShapeLayer::ShapeLayer (std::shared_ptr<ShapeRenderer> renderer, bool fixed)
    : Ren (renderer), ViewMatrix (glm::mat4()), Fixed (fixed)
  {
  }

  ShapeLayer::~ShapeLayer() { this->Shad->Disable(); }

  void ShapeLayer::Add (std::shared_ptr<Shape> shape) { this->Shapes.push_back (shape); }

  void ShapeLayer::Remove (std::shared_ptr<Shape> shape)
  {
    auto it = std::find (this->Shapes.begin(), this->Shapes.end(), shape);
    this->Shapes.erase (it);
  }

  void ShapeLayer::Render()
  {
    this->Shad->Enable();
    if (!this->Fixed)
    {
      this->Shad->SetMat4 ("view", this->ViewMatrix);
    }
    this->Ren->Begin();

    for (auto i : Shapes)
      this->Ren->Submit (*i);

    this->Ren->End();
    this->Ren->Draw();
  }

  void ShapeLayer::SetViewMatrix (glm::mat4 view) { this->ViewMatrix = view; }
} // namespace graphics
} // namespace stella
