#pragma once

#include "./map_grid.h"
#include "../scripting/basic_lua_api.h"

namespace stella
{
namespace core
{

  class TileMap
  {
    public:
      std::vector<std::shared_ptr<MapGrid>> layers;

    private:
      script::BasicLuaApi m_script_api;
      std::string m_name;
      unsigned m_number_of_layers = 0;
      unsigned m_width = 0;
      unsigned m_height = 0;

    public:
      TileMap(const std::string &path);
      ~TileMap();
      inline unsigned number_of_layers() const { return m_number_of_layers; }
      inline unsigned width() const { return m_width; }
      inline unsigned height() const { return m_height; }

    private:
      void load(const std::string &path);
  };

}
}

