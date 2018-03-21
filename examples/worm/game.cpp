#include "game.h"

Game::Game(stella::graphics::Display &display, stella::graphics::Shader *shader,
       std::array<bool, 1024> &keys) {
  systems.add<MovementSystem>();
  systems.add<PlayerMovementSystem>((int)display.GetWidth());
  systems.add<CollisionSystem>((int)display.GetWidth(),
                               (int)display.GetHeight());
  systems.add<CollectSystem>();
  // systems.add<LightingSystem>(shader);
  // systems.add<AnimationSystem>();
  systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(),
                            shader);
  systems.add<Worm>(entities, keys);
  systems.configure();

  this->AddCoins(8);
}

Game::~Game() {
  delete CoinTex;
}

void Game::AddCoins(size_t num_coins) {
  int offset = 2;
  CoinTex =
      new stella::graphics::Texture("coin-tex", "assets/sprites/snake16.png");

  for (size_t i = offset; i < num_coins * offset; i += offset) {
    auto coin = entities.create();
    coin.assign<TextureComponent>(16, 16, *CoinTex, 16);
    coin.assign<SpatialComponent>(16, 16, offset * 16 + i * 16, 304);
    coin.assign<BodyComponent>();
    coin.assign<CollectibleComponent>(CollectibleType::Seed);
  }
}

void Game::add_animation(entityx::Entity &ent, std::string name,
                         std::vector<unsigned int> frames,
                         unsigned int framerate) {
  entityx::ComponentHandle<TextureComponent> tex =
      ent.component<TextureComponent>();
  if (tex) {
    tex->sprite->Animations.Add(name, frames, framerate);
  }
}

void Game::Update(entityx::TimeDelta dt) {
  systems.update_all(dt);
  //worm->Update();
}
