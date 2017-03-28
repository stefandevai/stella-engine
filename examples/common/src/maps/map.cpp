#include "map.h"

namespace stella { namespace graphics {
  Map::Map(const char* filename, stella::graphics::Texture *texture)
    : Tex(texture)
  {
    this->load(filename); 
  }
  
  Map::~Map()
  {

  }

  void Map::load(const char* filename)
  {

  }

} }

