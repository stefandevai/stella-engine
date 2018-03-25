#include <iostream>
#include <array>
#include <stella/stella.h>

#include "game.h"

int main(int argc, char *argv[]) {
  stella::graphics::Display display(720, 405, "S T E L L A R");
  display.SetClearColor(0, 0, 0);
  
	stella::graphics::Shader contrastshader("assets/shaders/fbo_shader.vs", "assets/shaders/fbo_shader.fs");
	stella::graphics::Shader blurshader("assets/shaders/horizontal_blur.vs", "assets/shaders/horizontal_blur.fs");

	Framebuffer* blurFBO[2];
	blurFBO[0] = new Framebuffer(display);
	blurFBO[1] = new Framebuffer(display);

  Game game(display);


  while (display.IsRunning()) {
  	blurFBO[0]->Bind();
  	display.Clear();
    game.Update(display.GetDT());

		int amount = 10;
		bool horizontal = true;
		blurshader.Enable();
		for (int i = 1; i < amount; ++i) {
			blurFBO[horizontal]->Bind();
			blurshader.SetInt("horizontal", horizontal);
			blurFBO[!horizontal]->Draw();
			horizontal = !horizontal;
		}

		blurFBO[!horizontal]->Unbind();
		blurFBO[!horizontal]->Draw();
		blurshader.Disable();
    display.Update();
  }
	
	delete blurFBO[1];
	delete blurFBO[0];

  return 0;
}
