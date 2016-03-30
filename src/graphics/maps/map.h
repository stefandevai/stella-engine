#pragma once

#include <vector>
#include "../texture.h"

namespace stella { namespace graphics {
  class Map
  {
    public:
      virtual ~Map();
      virtual void SetPlatformFrames(const std::vector<unsigned int> single_frames);

      virtual void Update();
      virtual void AddPlatformsToLayer();
      virtual void AddBGToLayer();
      virtual void AddFGToLayer();

    protected:
      std::vector<unsigned int> Platforms;
      std::vector<std::vector<unsigned int>> BG;
      std::vector<std::vector<unsigned int>> FG;

      const Texture &Tex;
      const unsigned int &ScreenWidth, &ScreenHeight;
      unsigned int SWidthInTiles, SHeightInTiles;

      Map(const Texture &tex, const unsigned int &screenWidth, const unsigned int &screenHeight);
      virtual void generateMap();
  };
} }

