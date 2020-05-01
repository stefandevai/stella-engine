#include "stella/graphics/layers/shape_layer.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export

#include <algorithm>

namespace stella
{
namespace graphics
{
  ShapeLayer::ShapeLayer (std::shared_ptr<ShapeRenderer> renderer, bool fixed)
    : Layer (renderer, fixed)
  {
  }

  ShapeLayer::~ShapeLayer() { this->Shad->Disable(); }

  void ShapeLayer::Add (std::shared_ptr<Renderable> renderable) {
      auto shape = std::dynamic_pointer_cast<Shape> (renderable);
      if (shape != nullptr)
      {
          Add(shape);
      }
  }

  void ShapeLayer::Remove (std::shared_ptr<Renderable> renderable) {
    auto shape = std::dynamic_pointer_cast<Shape> (renderable);
    if (shape != nullptr)
    {
        Remove(shape);
    }
  }

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
      this->Ren->Submit (i);

    this->Ren->End();
    this->Ren->Draw();
  }
} // namespace graphics
} // namespace stella
