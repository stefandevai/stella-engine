#pragma once

#include <GL/glew.h>
#include <vector>

class Framebuffer {
	public:
		Framebuffer();
		~Framebuffer();

		void Bind();
		void Unbind();

		inline GLuint GetTexture() { return FBOtex; }

	private:
		GLuint FBOtex;
		GLuint FBO;

		void init();
};
	
