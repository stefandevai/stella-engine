#pragma once

#include <entt/entity/registry.hpp>
#include "./map_grid.h"
#include "./tile.h"
#include "../scripting/basic_lua_api.h"

namespace stella
{
namespace core
{

  class TileMap
  {
    public:
      std::vector<std::shared_ptr<MapGrid>> tile_layers;
      std::vector<std::shared_ptr<MapGrid>> collision_layers;

    private:
      entt::registry &m_registry;
      script::BasicLuaApi m_script_api;
      std::string m_path;
      std::string m_name;
      unsigned m_number_of_layers = 0;
      unsigned m_tile_dimension = 0;
      unsigned m_width = 0;
      unsigned m_height = 0;

    public:
      TileMap(const std::string &path, entt::registry &registry);
      ~TileMap();
      inline unsigned number_of_layers() const { return m_number_of_layers; }
      inline unsigned width() const { return m_width; }
      inline unsigned height() const { return m_height; }
      inline unsigned tile_dimension() const { return m_tile_dimension; }
      void create_tile_entity(const int x, const int y, const unsigned layer_id, const bool collidable);
      void create_tile_entities(const int beginx, const int endx, const int beginy, const int endy);

      void load(const std::string &path);
      void save(const std::string &path);
      void load_lua(const std::string &path);

  };

}
}

