#pragma once

#include "./basic_lua_api.hpp"
#include "./map_grid.hpp"
#include "./tile.hpp"
#include <entt/entity/registry.hpp> // IWYU pragma: export

namespace stella
{
namespace core
{
  class TileMap
  {
  public:
    std::vector<std::shared_ptr<MapGrid>> layers;

  private:
    std::string m_path;
    entt::registry& m_registry;
    script::BasicLuaApi m_script_api;
    std::string m_name;
    unsigned m_number_of_layers = 0;
    int m_tile_dimension   = 0;
    unsigned m_width            = 0;
    unsigned m_height           = 0;

  public:
    TileMap (const std::string& path, entt::registry& registry);
    ~TileMap();
    inline unsigned number_of_layers() const { return m_number_of_layers; }
    inline unsigned width() const { return m_width; }
    inline unsigned height() const { return m_height; }
    inline unsigned tile_dimension() const { return m_tile_dimension; }
    inline std::string get_name() const { return m_name; }
    inline std::string get_path() const { return m_path; }

    void set_name (const std::string& name) { m_name = name; }
    void update_tile (const int value, const int x, const int y, const unsigned layer_id, const bool collidable);
    void update_tile_sprite (entt::entity entity, const unsigned layer_id, const int value);
    void update_tile_position (entt::entity entity, const unsigned layer_id, const int x, const int y, const int z);
    void create_tile_entity (const int value, const int x, const int y, const int z, const unsigned layer_id, bool collidable = false);
    void create_tile_entities (const int beginx, const int endx, const int beginy, const int endy);
    void resize (const int top, const int right, const int bottom, const int left);
    void refresh();

    void load (const std::string& path);
    void save (const std::string& path);
    void load_lua (const std::string& path);
    void load_xml (const std::string& path);
    void clear();
  };

} // namespace core
} // namespace stella
