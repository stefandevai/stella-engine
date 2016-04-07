#include "map.h"

namespace stella { namespace graphics {
  Map::Map(const char* filename, const SpriteSheet &spritesheet)
    : Frames(spritesheet)
  {
    this->loadData(filename);   
  }
  
  Map::~Map()
  {

  }

  void Map::Update()
  {
    for (auto i : Layers)
      i->Update();
  }

  void Map::loadData(const char* filename)
  {

  }

} }

