#include "stella/graphics/layers/firelayer.hpp"

#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include <memory>

#include <glm/glm.hpp>                  // IWYU pragma: export
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace
{
void drawQuad (GLuint& VAO, GLuint& VBO)
{
  if (VAO == 0)
  {
    //// Quad
    float vertices[] = {
        // Positions   // Texture coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays (1, &VAO);
    glGenBuffers (1, &VBO);
    glBindVertexArray (VAO);
    glBindBuffer (GL_ARRAY_BUFFER, VBO);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*) 0);
    glEnableVertexAttribArray (1);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*) (3 * sizeof (float)));
  }

  glBindVertexArray (VAO);
  glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray (0);
}
} // namespace

namespace stella
{
namespace graphics
{
  FireLayer::FireLayer (graphics::Display& display)
    : SpriteLayer (std::shared_ptr<SpriteRenderer> (new SpriteRenderer()), false), Display (display)
  {
    this->QuadVAO = 0;
    this->QuadVBO = 0;

    // Initialize shader and textures IDs
    this->Shad =
        std::shared_ptr<Shader> (new Shader ("assets/shaders/sprite_batch.vert", "assets/shaders/sprite_batch.frag"));
    GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    this->Shad->Enable();
    const auto projection =
        glm::ortho (0.0f, (float) display.GetWidth(), (float) display.GetHeight(), 0.0f, -20.0f, 10.0f);
    this->Shad->SetMat4 ("proj", projection);
    this->Shad->SetIntv ("textures", tex_ids, 21);
    this->Shad->Disable();

    this->ShNormal   = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/no_effect.frag");
    this->ShContrast = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/contrast.frag");
    this->ShBlur     = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/blur.frag");
    this->ShBloom    = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/combine.frag");
    this->ShBloom->Enable();
    this->ShBloom->SetInt ("NormalScene", 0);
    this->ShBloom->SetInt ("PostScene", 1);
    this->ShBloom->Disable();

    this->NormalFBO   = new Framebuffer (display);
    this->ContrastFBO = new Framebuffer (display);
    this->BlurFBO[0]  = new Framebuffer (display);
    this->BlurFBO[1]  = new Framebuffer (display);
  }

  FireLayer::FireLayer (const FireLayer& copied_layer) : SpriteLayer (copied_layer.Ren), Display (copied_layer.Display)
  {
    QuadVAO = copied_layer.QuadVAO;
    QuadVBO = copied_layer.QuadVBO;

    ShNormal   = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/no_effect.frag");
    ShContrast = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/contrast.frag");
    ShBlur     = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/blur.frag");
    ShBloom    = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/combine.frag");
    ShBloom->Enable();
    ShBloom->SetInt ("NormalScene", 0);
    ShBloom->SetInt ("PostScene", 1);
    ShBloom->Disable();

    NormalFBO   = new Framebuffer (Display);
    ContrastFBO = new Framebuffer (Display);
    BlurFBO[0]  = new Framebuffer (Display);
    BlurFBO[1]  = new Framebuffer (Display);
  }

  FireLayer& FireLayer::operator= (const FireLayer& layer)
  {
    if (this != &layer)
    {
      QuadVAO = layer.QuadVAO;
      QuadVBO = layer.QuadVBO;

      ShNormal   = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/no_effect.frag");
      ShContrast = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/contrast.frag");
      ShBlur     = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/blur.frag");
      ShBloom    = new stella::graphics::Shader ("assets/shaders/no_effect.vert", "assets/shaders/combine.frag");
      ShBloom->Enable();
      ShBloom->SetInt ("NormalScene", 0);
      ShBloom->SetInt ("PostScene", 1);
      ShBloom->Disable();

      NormalFBO   = new Framebuffer (Display);
      ContrastFBO = new Framebuffer (Display);
      BlurFBO[0]  = new Framebuffer (Display);
      BlurFBO[1]  = new Framebuffer (Display);
    }

    return *this;
  }

  FireLayer::~FireLayer()
  {
    delete this->ShNormal;
    delete this->ShContrast;
    delete this->ShBlur;
    delete this->ShBloom;
    delete this->NormalFBO;
    delete this->ContrastFBO;
    delete this->BlurFBO[0];
    delete this->BlurFBO[1];
  }

  void FireLayer::Render() { this->RenderWithFBOs(); }

  void FireLayer::RenderScene()
  {
    this->Shad->Enable();
    if (!this->Fixed)
    {
      this->Shad->SetMat4 ("view", this->ViewMatrix);
    }
    this->Ren->Begin();

    // glm::mat4 trans;
    // trans = glm::translate(trans, glm::vec3(360.0f, 202.0f, 0.0f));
    // trans = glm::scale(trans, glm::vec3(0.9f, 0.9f, 1.0f));
    // trans = glm::rotate(trans, glm::radians(-5.0f), glm::vec3(0.0f,
    // 0.0f, 1.0f)); trans = glm::translate(trans, glm::vec3(-360.0f, -202.0f,
    // 0.0f)); this->Ren->PushTransformation(trans);

    for (auto i : Sprites)
      this->Ren->Submit (*i);

    // this->Ren->PopTransformation();

    this->Ren->End();
    this->Ren->Draw();
    this->Shad->Disable();
  }

  void FireLayer::RenderWithFBOs()
  {
    // glDisable(GL_DEPTH_TEST);
    this->NormalFBO->Bind();
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClear(GL_COLOR_BUFFER_BIT);
    this->RenderScene();
    glDisable (GL_DEPTH_TEST);
    this->ContrastFBO->Bind();
    this->ShNormal->Enable();
    this->NormalFBO->ActivateTexture (GL_TEXTURE0);
    drawQuad (this->QuadVAO, this->QuadVBO);

    this->BlurFBO[0]->Bind();
    this->ShContrast->Enable();
    this->ContrastFBO->ActivateTexture (GL_TEXTURE0);
    drawQuad (this->QuadVAO, this->QuadVBO);

    int amount      = 4;
    bool horizontal = true;
    this->ShBlur->Enable();
    for (int i = 0; i < amount; ++i)
    {
      this->BlurFBO[horizontal]->Bind();
      this->ShBlur->SetInt ("horizontal", horizontal);
      this->BlurFBO[!horizontal]->ActivateTexture (GL_TEXTURE0);
      drawQuad (this->QuadVAO, this->QuadVBO);
      horizontal = !horizontal;
    }
    BlurFBO[!horizontal]->Unbind();

    this->ShBloom->Enable();
    this->NormalFBO->ActivateTexture (GL_TEXTURE0);
    this->BlurFBO[!horizontal]->ActivateTexture (GL_TEXTURE1);
    drawQuad (this->QuadVAO, this->QuadVBO);
    glEnable (GL_DEPTH_TEST);
    // glViewport(0.0f, 0.0f, this->Display.GetWidth(),
    // this->Display.GetHeight());
  }
} // namespace graphics
} // namespace stella
