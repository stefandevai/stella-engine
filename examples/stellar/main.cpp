#include <iostream>
#include <array>
#include <stella/stella.h>

#include "game.h"

void DrawQuad(GLuint &VAO, GLuint &VBO) {
	if (VAO == 0) {
		//// Quad
		float vertices[] = {
				// Positions   // Texture coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

int main(int argc, char *argv[]) {
	GLuint quadVAO = 0;
	GLuint quadVBO;

  stella::graphics::Display display(720, 405, "S T E L L A R");
  display.SetClearColor(0, 0, 0);
  
	stella::graphics::Shader normalshader("assets/shaders/fbo_shader.vs", "assets/shaders/normal_shader.fs");
	stella::graphics::Shader contrastshader("assets/shaders/fbo_shader.vs", "assets/shaders/fbo_shader.fs");
	stella::graphics::Shader blurshader("assets/shaders/horizontal_blur.vs", "assets/shaders/horizontal_blur.fs");
	stella::graphics::Shader bloomshader("assets/shaders/fbo_shader.vs", "assets/shaders/combine.fs");
	bloomshader.Enable();
	bloomshader.SetInt("NormalScene", 0);
	bloomshader.SetInt("PostScene", 1);
	bloomshader.Disable();

	Framebuffer normalFBO(display);
	Framebuffer contrastFBO(display);
	Framebuffer* blurFBO[2];
	blurFBO[0] = new Framebuffer(display);
	blurFBO[1] = new Framebuffer(display);

  Game game(display);

  while (display.IsRunning()) {
		normalFBO.Bind();
  	display.Clear();
    game.Update(display.GetDT());
		contrastFBO.Bind();
		normalshader.Enable();
		normalFBO.ActivateTexture(GL_TEXTURE0);
		DrawQuad(quadVAO, quadVBO);

		blurFBO[0]->Bind();
		contrastshader.Enable();
		contrastFBO.ActivateTexture(GL_TEXTURE0);
		DrawQuad(quadVAO, quadVBO);

		int amount = 2;
		bool horizontal = true;
		blurshader.Enable();
		for (int i = 0; i < amount; ++i) {
			blurFBO[horizontal]->Bind();
			blurshader.SetInt("horizontal", horizontal);
			blurFBO[!horizontal]->ActivateTexture(GL_TEXTURE0);
			DrawQuad(quadVAO, quadVBO);
			horizontal = !horizontal;
		}
		blurFBO[!horizontal]->Unbind();

		display.Clear();
		bloomshader.Enable();
		normalFBO.ActivateTexture(GL_TEXTURE0);
		blurFBO[!horizontal]->ActivateTexture(GL_TEXTURE1);
		DrawQuad(quadVAO, quadVBO);
    display.Update();
  }
	
	delete blurFBO[1];
	delete blurFBO[0];

  return 0;
}

