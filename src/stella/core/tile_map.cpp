#include "stella/core/tile_map.h"
#include "stella/components/sprite_component.h"
#include "stella/components/position_component.h"
#include "stella/components/dimension_component.h"
#include "stella/components/tile_component.h"
#include "stella/components/log_component.h"

#include <cmath>
#include <glm/glm.hpp>

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>

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
    m_width = map_table["size"]["width"];
    m_height = map_table["size"]["height"];
    assert(m_number_of_layers > 0);
    assert(map_table["layers"] != sol::lua_nil);
    assert(m_width > 0);
    assert(m_height > 0);

    for (unsigned int i = 1; i <= m_number_of_layers; ++i)
    {
      assert(map_table["layers"][i] != sol::lua_nil);
      auto layer = std::make_shared<MapGrid>(m_width, m_height);
      layer->set_texture_name(map_table["layers"][i]["texture"]);
      layer->set_render_layer_name(map_table["layers"][i]["render_layer"]);
      layer->set_collision(map_table["layers"][i]["collision"]);

      for (uint y = 0; y < m_height; ++y)
      {
        for (uint x = 1; x <= m_width; ++x)
        {
          int value = map_table["layers"][i]["grid"][x + y*m_width][1];
          int collidable = map_table["layers"][i]["grid"][x + y*m_width][2];
          Tile tile{value};
          tile.x = x-1;
          tile.y = y;
          tile.z = map_table["layers"][i]["grid"][x + y*m_width][3];
          tile.collidable = (bool)collidable;

          // Checks surrounding tiles to set active tile edges
          // Checks tile to the top
          if (map_table["layers"][i]["grid"][(x+1) + y*m_width] != sol::lua_nil && map_table["layers"][i]["grid"][x + (y-1)*m_width][2] == 0)
          {
            tile.active_edges.set(0);
          }
          // Checks tile to the right
          if (map_table["layers"][i]["grid"][(x+1) + y*m_width] != sol::lua_nil && map_table["layers"][i]["grid"][(x+1) + y*m_width][2] == 0)
          {
            tile.active_edges.set(1);
          }
          // Checks tile to the bottom
          if (map_table["layers"][i]["grid"][(x+1) + y*m_width] != sol::lua_nil && map_table["layers"][i]["grid"][(x) + (y+1)*m_width][2] == 0)
          {
            tile.active_edges.set(2);
          }
          // Checks tile to the left
          if (map_table["layers"][i]["grid"][(x+1) + y*m_width] != sol::lua_nil && map_table["layers"][i]["grid"][(x-1) + y*m_width][2] == 0)
          {
            tile.active_edges.set(3);
          }
          
          layer->set_value(x-1, y, tile);
        }
      }

      layers.emplace_back(layer);

      // if (layer->is_collision_grid())
      // {
      //   collision_layers.emplace_back(layer);
      // }
      // else
      // {
      //   tile_layers.emplace_back(layer);
      // }
    }

    std::cout << "Loaded TileMap: " << m_name << " from " << path << '\n';
  }

  void TileMap::load_lua(const std::string &path)
  {
    m_path = path;
    std::ifstream is(path);
    cereal::XMLInputArchive archive(is);
    archive(CEREAL_NVP(m_name),
            CEREAL_NVP(m_number_of_layers),
            CEREAL_NVP(m_tile_dimension),
            CEREAL_NVP(m_width),
            CEREAL_NVP(m_height),
            CEREAL_NVP(layers));
            // CEREAL_NVP(tile_layers),
            // CEREAL_NVP(collision_layers));

    std::cout << "Loaded TileMap: " << m_name << " from " << path << '\n';
  }
  
  void TileMap::save(const std::string &path)
  {
    m_path = path;
    std::ofstream os(path);
    cereal::XMLOutputArchive archive(os);

    archive(CEREAL_NVP(m_name),
            CEREAL_NVP(m_number_of_layers),
            CEREAL_NVP(m_tile_dimension),
            CEREAL_NVP(m_width),
            CEREAL_NVP(m_height),
            CEREAL_NVP(layers));
            // CEREAL_NVP(tile_layers),
            // CEREAL_NVP(collision_layers));

    std::cout << "Saved TileMap: " << m_name << " in " << path << '\n';
  }

  void TileMap::update_tile(const int value, const int x, const int y, const unsigned layer_id, const bool collidable)
  {
    // TODO
    //tile_layers[layer_id]->set_value(x/m_tile_dimension, y/m_tile_dimension, value);
    //create_tile_entity(x, y, layer_id, collidable);
    
  }

  // void TileMap::create_tile_entity(const int x, const int y, const unsigned layer_id, const bool collidable)
  // {
  //   int tx = x/m_tile_dimension;
  //   int ty = y/m_tile_dimension;

  //   if (collidable)
  //   {
  //     const auto &layer_tile = collision_layers[layer_id]->get_value(tx, ty);
  //     collision_layers[layer_id]->set_visibility(tx, ty, true);
  //     //layer_tile.visible = true;
  //     if (layer_tile.value > 0)
  //     {
  //       auto tile = m_registry.create();
  //       m_registry.assign<components::TileComponent>(tile, layer_id, true);
  //       m_registry.assign<components::SpriteComponent>(tile, collision_layers[layer_id]->get_texture_name(), glm::vec2(m_tile_dimension, m_tile_dimension), collision_layers[layer_id]->get_render_layer_name(), 0);
  //       m_registry.assign<components::PositionComponent>(tile, tx*32, ty*32);
  //       m_registry.assign<components::DimensionComponent>(tile, m_tile_dimension, m_tile_dimension);
  //     }
  //   }
  //   else
  //   {
  //     const auto &layer_tile = tile_layers[layer_id]->get_value(tx, ty);
  //     tile_layers[layer_id]->set_visibility(tx, ty, true);
  //     if (layer_tile.value > 0)
  //     {
  //       auto tile = m_registry.create();
  //       //layer_tile.visible = true;
  //       m_registry.assign<components::TileComponent>(tile, layer_id, false);
  //       m_registry.assign<components::SpriteComponent>(tile, tile_layers[layer_id]->get_texture_name(), glm::vec2(m_tile_dimension, m_tile_dimension), tile_layers[layer_id]->get_render_layer_name(), 0);
  //       m_registry.assign<components::PositionComponent>(tile, tx*m_tile_dimension, ty*m_tile_dimension);
  //       m_registry.assign<components::DimensionComponent>(tile, m_tile_dimension, m_tile_dimension);
  //     }
  //   }

  // }

  void TileMap::create_tile_entities(const int beginx, const int endx, const int beginy, const int endy)
  {
    assert(beginx < endx);
    assert(beginy < endy);
    int left = beginx / m_tile_dimension;
    int right = endx / m_tile_dimension;
    int top = beginy / m_tile_dimension;
    int bottom = ceil(endy / static_cast<double>(m_tile_dimension));
    int counter = 0;

    for (const auto &layer : this->layers)
    {
      for (auto y = top; y < bottom; ++y)
      {
        for (auto x = left; x < right; ++x)
        {
          const auto &layer_tile = layer->get_value(x, y);

          if (!layer_tile.visible && layer_tile.value > 0)
          {
            layer->set_visibility(x, y, true);
            auto tile = m_registry.create();
            m_registry.assign<components::TileComponent>(tile, counter, false);
            m_registry.assign<components::SpriteComponent>(tile, layer->get_texture_name(), glm::vec2(m_tile_dimension, m_tile_dimension), layer->get_render_layer_name(), layer_tile.value);
            m_registry.assign<components::PositionComponent>(tile, x*m_tile_dimension, y*m_tile_dimension);
            m_registry.assign<components::DimensionComponent>(tile, m_tile_dimension, m_tile_dimension);
          }
          else if (!layer_tile.visible)
          {
            layer->set_visibility(x, y, true);
          }
        }
      }
      ++counter;
    }

    // counter = 0;
    // for (const auto &layer : this->collision_layers)
    // {
    //   for (auto y = top; y < bottom; ++y)
    //   {
    //     for (auto x = left; x < right; ++x)
    //     {
    //       const auto &layer_tile = layer->get_value(x, y);
    //       if (!layer_tile.visible && layer_tile.value > 0)
    //       {
    //         layer->set_visibility(x, y, true);
    //         auto tile = m_registry.create();
    //         m_registry.assign<components::TileComponent>(tile, counter, true);
    //         m_registry.assign<components::SpriteComponent>(tile, layer->get_texture_name(), glm::vec2(m_tile_dimension, m_tile_dimension), layer->get_render_layer_name(), layer_tile.value);
    //         m_registry.assign<components::PositionComponent>(tile, x*m_tile_dimension, y*m_tile_dimension);
    //         m_registry.assign<components::DimensionComponent>(tile, m_tile_dimension, m_tile_dimension);
    //         m_registry.assign<components::LogComponent>(tile);
    //       }
    //       else if (!layer_tile.visible)
    //       {
    //         layer->set_visibility(x, y, true);
    //       }
    //     }
    //   }
    //   ++counter;
    // }
  }
}
}

