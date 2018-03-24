#include <iostream>
#include <array>
#include <stella/stella.h>

#include "game.h"

int main(int argc, char *argv[]) {
  stella::graphics::Display display(720, 405, "S T E L L A R");
  display.SetClearColor(0, 0, 0);

	Framebuffer FBO;
	stella::graphics::Shader FBOshader("assets/shaders/fbo_shader.vs", "assets/shaders/fbo_shader.fs");

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
	};
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  Game game(display);

  while (display.IsRunning()) {
		FBO.Bind();
    display.Clear();
    game.Update(display.GetDT());
		FBO.Unbind();
		display.SetClearColor(0.5f, 0.3f, 0.2f);
		display.Clear();
		FBOshader.Enable();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, FBO.GetTexture());
		glDrawArrays(GL_TRIANGLES, 0, 6);
    display.Update();
  }

  return 0;
}
