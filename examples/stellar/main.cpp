#include <iostream>
#include <array>
#include <stella/stella.h>

#include "game.h"

int main(int argc, char *argv[]) {
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
		normalFBO.Draw();

  	blurFBO[0]->Bind();
  	contrastshader.Enable();
		contrastFBO.ActivateTexture(GL_TEXTURE0);
  	contrastFBO.Draw();

		int amount = 10;
		bool horizontal = true;
		blurshader.Enable();
		for (int i = 1; i < amount; ++i) {
			blurFBO[horizontal]->Bind();
			blurshader.SetInt("horizontal", horizontal);
			blurFBO[!horizontal]->ActivateTexture(GL_TEXTURE0);
			blurFBO[!horizontal]->Draw();
			horizontal = !horizontal;
		}

		blurFBO[!horizontal]->Unbind();

		display.Clear();
		bloomshader.Enable();
		normalFBO.ActivateTexture(GL_TEXTURE0);
		blurFBO[!horizontal]->ActivateTexture(GL_TEXTURE1);
		blurFBO[!horizontal]->Draw();
    display.Update();
  }
	
	delete blurFBO[1];
	delete blurFBO[0];

  return 0;
}
