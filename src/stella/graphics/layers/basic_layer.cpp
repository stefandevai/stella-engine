#include "stella/graphics/layers/basic_layer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace stella
{
namespace graphics
{
  BasicLayer::BasicLayer (float width, float height, const char* vspath, const char* fspath, bool fixed)
    : Layer (std::shared_ptr<Renderer> (new Renderer()), fixed)
  {
    // Initialize shader and textures IDs
    this->Shad        = std::shared_ptr<Shader> (new Shader (vspath, fspath));
    GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    this->Shad->Enable();
    const auto projection = glm::ortho (0.0f, width, height, 0.0f, -20.0f, 10.0f);
    this->Shad->SetMat4 ("proj", projection);
    this->Shad->SetIntv ("textures", tex_ids, 21);
    this->Shad->Disable();
  }
  BasicLayer::~BasicLayer() {}
} // namespace graphics
} // namespace stella
