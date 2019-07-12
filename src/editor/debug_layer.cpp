#include "editor/debug_layer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stella/graphics/opengl.h"

namespace stella {
namespace editor {
DebugLayer::DebugLayer(float width, float height, bool fixed)
  : Layer(std::make_shared<graphics::Renderer>(), fixed) {
    // Initialize shader and textures IDs
    this->Shad = std::make_shared<graphics::Shader>("assets/shaders/sprite_batch.vert", "assets/shaders/sprite_batch.frag");
    GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    this->Shad->Enable();
    const auto projection = glm::ortho(0.0f, width, height, 0.0f, -20.0f, 10.0f);
    this->Shad->SetMat4("proj", projection);
    this->Shad->SetIntv("textures", tex_ids, 21);
    this->Shad->Disable();
}
DebugLayer::~DebugLayer() {}

void DebugLayer::Render() {
  glDisable(GL_DEPTH_TEST);
  this->Shad->Enable();
  this->Ren->Begin();

  //glm::mat4 trans;
  //trans = glm::translate(trans, glm::vec3(360.0f, 202.0f, 0.0f));
  //trans = glm::scale(trans, glm::vec3(0.9f, 0.9f, 1.0f));
  //trans = glm::rotate(trans, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  //trans = glm::translate(trans, glm::vec3(-360.0f, -202.0f, 0.0f));
  //this->Ren->PushTransformation(trans);

  for (auto i : Sprites)
    this->Ren->Submit(*i);
  
  //this->Ren->PopTransformation();

  this->Ren->End();
  this->Ren->Draw();
  glEnable(GL_DEPTH_TEST);
	//this->Shad->Disable();
}
} // namespace graphics
} // namespace stella

