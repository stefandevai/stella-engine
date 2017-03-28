#include <entityx/entityx.h>
#include <stella/stella.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scenelayer.h"

struct Position 
{
  Position(int x = 0, int y = 0) : x(x), y(y) {}
  int x, y;
};

struct TextureComponent
{
	inline TextureComponent(int w, int h, stella::graphics::Texture &texture, int frame = 0) { sprite = new stella::graphics::Sprite(0, 0, w, h, texture, frame); InLayer = false; }
	stella::graphics::Sprite *sprite;
	bool InLayer;
};

struct RenderSystem : public entityx::System<RenderSystem> {
	SceneLayer*	TileLayer;
	stella::graphics::Shader *shader;

	explicit RenderSystem(int width, int height, stella::graphics::Shader *shad) {
		this->shader = shad;
		glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
		this->TileLayer = new SceneLayer(shad, proj);
	}

	~RenderSystem() {
		delete this->TileLayer;
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {
		es.each<Position, TextureComponent>([this](entityx::Entity entity, Position &pos, TextureComponent &tex) {
			tex.sprite->Pos.x = pos.x;
			tex.sprite->Pos.y = pos.y;
			if (!tex.InLayer) {
				this->TileLayer->Add(tex.sprite);
				tex.InLayer = true;
				std::cout << tex.sprite->Pos.x << std::endl;
			}
			this->TileLayer->Render();
		});

	};
};

class Application : public entityx::EntityX {
	public:
		stella::graphics::Texture *StellaTex;

		explicit Application(stella::graphics::Display &display, stella::graphics::Shader *shader) {
			systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(), shader);
			systems.configure();
			  
			entityx::Entity player = entities.create();
			player.assign<Position>((int)display.GetWidth()/2 - 23, (int)display.GetHeight()/2 - 51);

	    StellaTex = new stella::graphics::Texture("stella-ttex", "assets/gfx/sprites/tina.png");
			player.assign<TextureComponent>(46, 102, *StellaTex, 0);
		}

		~Application() {
		}

		void update(entityx::TimeDelta dt) {
			systems.update_all(dt);
		}
};

int main(int argc, char *argv[])
{
  GLboolean Keys[1024] = { 0 };
  stella::graphics::Display display(800, 600, "Stella", Keys);
  display.SetClearColor(22, 38, 47);

  // TODO Move this block to init on singleton class
  GLint tex_ids[] =
  {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
  };

  stella::graphics::Shader shader("assets/shaders/basic_shader.vsh", "assets/shaders/basic_shader.fsh");
  shader.Enable();
  shader.SetIntv("textures", tex_ids, 10);
  shader.Disable();
  // End of block

  Application app(display, &shader);

  while(display.IsRunning())
  {
    display.Clear();

		app.update((entityx::TimeDelta)display.GetDT());
    display.Update();
  }

  return 0;
}

