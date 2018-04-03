#include "firelayer.h"

FireLayer::FireLayer(stella::graphics::Shader *shader, const glm::mat4 &projection, stella::graphics::Display& display)
	: Layer(new stella::graphics::Renderer(), shader, projection), Display(display) {

	this->QuadVAO = 0;

	this->ShNormal = new stella::graphics::Shader("assets/shaders/no_effect.vert", "assets/shaders/no_effect.frag");
	this->ShContrast = new stella::graphics::Shader("assets/shaders/no_effect.vert", "assets/shaders/contrast.frag");
	this->ShBlur = new stella::graphics::Shader("assets/shaders/no_effect.vert", "assets/shaders/blur.frag");
	this->ShBloom = new stella::graphics::Shader("assets/shaders/no_effect.vert", "assets/shaders/combine.frag");
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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		this->Render();
    this->ContrastFBO->Bind();
		this->ShNormal->Enable();
		this->NormalFBO->ActivateTexture(GL_TEXTURE0);
		this->DrawQuad(this->QuadVAO, this->QuadVBO);

		this->BlurFBO[0]->Bind();
    this->ShContrast->Enable();
    this->ContrastFBO->ActivateTexture(GL_TEXTURE0);
    this->DrawQuad(this->QuadVAO, this->QuadVBO);

    int amount = 4;
    bool horizontal = true;
    this->ShBlur->Enable();
    for (int i = 0; i < amount; ++i) {
      this->BlurFBO[horizontal]->Bind();
      this->ShBlur->SetInt("horizontal", horizontal);
      this->BlurFBO[!horizontal]->ActivateTexture(GL_TEXTURE0);
      this->DrawQuad(this->QuadVAO, this->QuadVBO);
      horizontal = !horizontal;
    }
    BlurFBO[!horizontal]->Unbind();

    this->ShBloom->Enable();
    this->NormalFBO->ActivateTexture(GL_TEXTURE0);
    this->BlurFBO[!horizontal]->ActivateTexture(GL_TEXTURE1);
		this->DrawQuad(this->QuadVAO, this->QuadVBO);
    glViewport(0.0f, 0.0f, this->Display.GetWidth(), this->Display.GetHeight());
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

