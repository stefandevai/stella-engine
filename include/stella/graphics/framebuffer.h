#pragma once

#include <GL/glew.h>
#include <vector>

#include "stella/graphics/display.h"
#include "stella/graphics/shader.h"

class Framebuffer {
	public:
		Framebuffer(stella::graphics::Display &display);
		~Framebuffer();

		void Bind();
		void Unbind();
		void Draw();

		void ActivateTexture(GLenum tex_id);
		inline GLuint GetTexture() { return FBOtex; }

	private:
		GLuint FBO, FBOtex, VAO, VBO;
		stella::graphics::Display &Display;

		void init();
};
	
