#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "renderer.h"
#include "shader.h"

namespace stella { namespace graphics {
  class Layer {
    public:
      virtual ~Layer();
      virtual void Add(Sprite* sprite);
      virtual void Render();

    protected:
      Renderer *Ren;
      Shader *Shad;
      glm::mat4 Projection;
      std::vector<Sprite*> Sprites;

      Layer(Renderer* renderer, Shader* shader, glm::mat4 projection);
  };
} }

