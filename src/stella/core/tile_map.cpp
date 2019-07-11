#include "stella/core/tile_map.h"
#include "stella/components/sprite_component.h"
#include "stella/components/position_component.h"
#include "stella/components/dimension_component.h"
#include "stella/components/tile_component.h"

#include <glm/glm.hpp>

namespace stella
{
namespace core
{

  TileMap::TileMap(const std::string &path, entt::registry &registry)
    : m_registry(registry)
  {
    this->load(path);
  }

  TileMap::~TileMap()
  {

  }

  void TileMap::load(const std::string &path)
  {
    m_script_api.run_script(path);
    const sol::table &map_table = m_script_api.get_variable<sol::table>("Map");
    m_name = map_table["name"] == sol::lua_nil ? path : map_table["name"];
    m_number_of_layers = map_table["number_of_layers"];
    m_tile_dimension = map_table["tile_dimension"];
    assert(map_table["size"] != sol::lua_nil);
    const unsigned m_width = map_table["size"]["width"];
    const unsigned m_height = map_table["size"]["height"];
    assert(m_number_of_layers > 0);
    assert(map_table["layers"] != sol::lua_nil);
    assert(m_width > 0);
    assert(m_height > 0);

    for (auto i = 1; i <= m_number_of_layers; ++i)
    {
      assert(map_table["layers"][i] != sol::lua_nil);
      auto layer = std::make_shared<MapGrid>(m_width, m_height);
      layer->set_texture_name(map_table["layers"][i]["texture"]);
      layer->set_render_layer_name(map_table["layers"][i]["render_layer"]);
      layer->set_collision(map_table["layers"][i]["collision"]);

      for (auto y = 0; y < m_height; ++y)
      {
        for (auto x = 1; x <= m_width; ++x)
        {
          int value = map_table["layers"][i]["grid"][x + y*m_width];
          layer->set_value(x-1, y, value);
        }
      }

      if (layer->is_collision_grid())
      {
        collision_layers.emplace_back(layer);
      }
      else
      {
        tile_layers.emplace_back(layer);
      }
    }

    std::cout << "Loaded TileMap: " << m_name << '\n';
  }

  void TileMap::create_tile_entity(const int x, const int y, const unsigned layer_id)
  {

  }

  void TileMap::create_tile_entities(const int beginx, const int endx, const int beginy, const int endy)
  {
    int left = beginx / m_tile_dimension;
    int right = endx / m_tile_dimension;
    int top = beginy / m_tile_dimension;
    int bottom = endy / m_tile_dimension;

    for (const auto &layer : this->tile_layers)
    {
      for (auto y = top; y < bottom; ++y)
      {
        for (auto x = left; x < right; ++x)
        {
          const auto value = layer->get_value(x, y);
          if (value > 0)
          {
            auto tile = m_registry.create();
            m_registry.assign<components::SpriteComponent>(tile, "tiles", glm::vec2(m_tile_dimension, m_tile_dimension), layer->get_render_layer_name(), 0);
            m_registry.assign<components::PositionComponent>(tile, x*m_tile_dimension, y*m_tile_dimension);
            m_registry.assign<components::DimensionComponent>(tile, m_tile_dimension, m_tile_dimension);
          }
        }
      }
    }

    for (const auto &layer : this->collision_layers)
    {
      for (auto y = top; y < bottom; ++y)
      {
        for (auto x = left; x < right; ++x)
        {
          const auto value = layer->get_value(x, y);
          if (value > 0)
          {
            auto tile = m_registry.create();
            m_registry.assign<components::SpriteComponent>(tile, "tiles", glm::vec2(m_tile_dimension, m_tile_dimension), layer->get_render_layer_name(), 0);
            m_registry.assign<components::PositionComponent>(tile, x*m_tile_dimension, y*m_tile_dimension);
            m_registry.assign<components::DimensionComponent>(tile, m_tile_dimension, m_tile_dimension);
          }
        }
      }
    }

  }

}
}

