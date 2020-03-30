#pragma once

#include "../framebuffer.hpp"
#include "../shader.hpp"
#include "./layer.hpp"

namespace stella
{
namespace graphics
{
  class FireLayer : public stella::graphics::Layer
  {
  public:
    explicit FireLayer (graphics::Display& display);
    FireLayer (const FireLayer& copied_layer);
    FireLayer& operator= (const FireLayer& layer);
    ~FireLayer();

    void Render() override;
    void RenderScene();
    void RenderWithFBOs();

  protected:
    GLuint QuadVAO, QuadVBO;
    Framebuffer *NormalFBO, *ContrastFBO, *BlurFBO[2];
    stella::graphics::Shader *ShNormal, *ShBlur, *ShContrast, *ShBloom;
    graphics::Display& Display;
  };
} // namespace graphics
} // namespace stella
