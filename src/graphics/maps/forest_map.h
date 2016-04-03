#pragma once

#include "map.h"

namespace stella { namespace graphics{
  class ForestMap : public Map
  {
    public:
      ForestMap(const Texture &tex, const unsigned int &screenWidth, const unsigned int &screenHeight);
      ~ForestMap();

      void Update();

    private:
      void generateFirstMap();
      void generateMap();
  };
} }

