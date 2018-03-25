#include "firelayer.h"

FireLayer::FireLayer(stella::graphics::Shader *shader, const glm::mat4 &projection, stella::graphics::Display& display)
	: Layer(new stella::graphics::Renderer(), shader, projection), Display(display) {

	this->QuadVAO = 0;

	this->ShNormal = new stella::graphics::Shader("assets/shaders/fbo_shader.vs", "assets/shaders/normal_shader.fs");
	this->ShContrast = new stella::graphics::Shader("assets/shaders/fbo_shader.vs", "assets/shaders/fbo_shader.fs");
	this->ShBlur = new stella::graphics::Shader("assets/shaders/horizontal_blur.vs", "assets/shaders/horizontal_blur.fs");
	this->ShBloom = new stella::graphics::Shader("assets/shaders/fbo_shader.vs", "assets/shaders/combine.fs");
	this->ShBloom->Enable();
	this->ShBloom->SetInt("NormalScene", 0);
	this->ShBloom->SetInt("PostScene", 1);
	this->ShBloom->Disable();

	this->NormalFBO = new Framebuffer(display);
	this->ContrastFBO = new Framebuffer(display);
	this->BlurFBO[0] = new Framebuffer(display);
	this->BlurFBO[1] = new Framebuffer(display);
}

FireLayer::~FireLayer() {
	delete this->ShNormal;
	delete this->ShContrast;
	delete this->ShBlur;
	delete this->ShBloom;
	delete this->NormalFBO;
	delete this->ContrastFBO;
	delete this->BlurFBO[0];
	delete this->BlurFBO[1];
}

void FireLayer::RenderWithFBOs() {
		this->NormalFBO->Bind();
		Display.Clear();
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		this->Render();
		this->ContrastFBO->Bind();
		this->ShNormal->Enable();
		this->NormalFBO->ActivateTexture(GL_TEXTURE0);
		DrawQuad(QuadVAO, QuadVBO);

		BlurFBO[0]->Bind();
		ShContrast->Enable();
		ContrastFBO->ActivateTexture(GL_TEXTURE0);
		DrawQuad(QuadVAO, QuadVBO);

		int amount = 4;
		bool horizontal = true;
		ShBlur->Enable();
		for (int i = 0; i < amount; ++i) {
			BlurFBO[horizontal]->Bind();
			ShBlur->SetInt("horizontal", horizontal);
			BlurFBO[!horizontal]->ActivateTexture(GL_TEXTURE0);
			DrawQuad(QuadVAO, QuadVBO);
			horizontal = !horizontal;
		}
		BlurFBO[!horizontal]->Unbind();

		this->ShBloom->Enable();
		NormalFBO->ActivateTexture(GL_TEXTURE0);
		BlurFBO[!horizontal]->ActivateTexture(GL_TEXTURE1);
		this->DrawQuad(QuadVAO, QuadVBO);
}

void FireLayer::DrawQuad(GLuint &VAO, GLuint &VBO) {
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

