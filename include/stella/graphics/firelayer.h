#pragma once

#include "layer.h"
#include "shader.h"
#include "framebuffer.h"

namespace stella {
namespace graphics {
class FireLayer : public stella::graphics::Layer {
public:
  FireLayer(stella::graphics::Shader *shader, const glm::mat4 &projection, stella::graphics::Display& display);
  ~FireLayer();

  void RenderWithFBOs();

private:
	GLuint QuadVAO, QuadVBO;
	Framebuffer *NormalFBO, *ContrastFBO, *BlurFBO[2];
	stella::graphics::Shader *ShNormal, *ShBlur, *ShContrast, *ShBloom;
	stella::graphics::Display& Display;

	void DrawQuad(GLuint &VAO, GLuint &VBO);
};
} // namespace graphics
} // namespace stella

