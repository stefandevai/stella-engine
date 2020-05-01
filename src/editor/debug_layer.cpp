#include "editor/debug_layer.hpp"

#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include "stella/graphics/shape_renderer.hpp"
#include <glm/glm.hpp>                  // IWYU pragma: export
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace stella
{
namespace editor
{
  DebugLayer::DebugLayer (float width, float height, bool fixed)
    : ShapeLayer (std::make_shared<graphics::ShapeRenderer>(), fixed)
  {
    // Initialize shader and textures IDs
    this->Shad =
        std::make_shared<graphics::Shader> ("assets/shaders/debug_shader.vert", "assets/shaders/debug_shader.frag");
    GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    this->Shad->Enable();
    const auto projection = glm::ortho (0.0f, width, height, 0.0f, -20.0f, 10.0f);
    this->Shad->SetMat4 ("proj", projection);
    this->Shad->SetIntv ("textures", tex_ids, 21);
    this->Shad->Disable();
  }
  DebugLayer::~DebugLayer() {}

  void DebugLayer::Render()
  {
    glDisable (GL_DEPTH_TEST);
    this->Shad->Enable();
    this->Ren->Begin();

    for (auto i : Shapes)
      this->Ren->Submit (*i);

    this->Ren->End();
    this->Ren->Draw();
    glEnable (GL_DEPTH_TEST);
  }
} // namespace editor
} // namespace stella
