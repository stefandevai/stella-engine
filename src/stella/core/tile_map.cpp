#include "stella/core/tile_map.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/log.hpp"
#include "stella/components/position.hpp"
#include "stella/components/sprite.hpp"
#include "stella/components/tile.hpp"

#include <cmath>
#include <glm/glm.hpp> // IWYU pragma: export

#include <cereal/archives/xml.hpp> // IWYU pragma: export
#include <cereal/cereal.hpp>       // IWYU pragma: export
#include <cereal/types/memory.hpp> // IWYU pragma: export
#include <cereal/types/vector.hpp> // IWYU pragma: export
#include <fstream>
#include <algorithm>

namespace stella
{
namespace core
{
  entt::registry* Tile::registry = nullptr;

  TileMap::TileMap (const std::string& path, entt::registry& registry) : m_path (path), m_registry (registry)
  {
    this->load (path);
    Tile::registry = &m_registry;
  }

  TileMap::~TileMap() {}

  void TileMap::load (const std::string& path)
  {
    auto pos                    = path.find_last_of ('.');
    const std::string extension = path.substr (pos + 1);
    if (extension == "lua")
    {
      m_path = path;
      load_lua (path);
    }
    else if (extension == "xml")
    {
      m_path = path;
      load_xml (path);
    }
    else
    {
      std::cout << "ERROR: Unknown file extension \"" + extension + "\"\n";
    }
  }

  void TileMap::load_lua (const std::string& path)
  {
    m_script_api.run_script (path);
    const sol::table& map_table = m_script_api.get_variable<sol::table> ("Map");
    m_name                      = map_table["name"] == sol::lua_nil ? path : map_table["name"];
    m_number_of_layers          = map_table["number_of_layers"];
    m_tile_dimension            = map_table["tile_dimension"];
    assert (map_table["size"] != sol::lua_nil);
    m_width  = map_table["size"]["width"];
    m_height = map_table["size"]["height"];
    assert (m_number_of_layers > 0);
    assert (map_table["layers"] != sol::lua_nil);
    assert (m_width > 0);
    assert (m_height > 0);

    for (unsigned int i = 1; i <= m_number_of_layers; ++i)
    {
      assert (map_table["layers"][i] != sol::lua_nil);
      auto layer = std::make_shared<MapGrid> (m_width, m_height);
      layer->set_texture_name (map_table["layers"][i]["texture"]);
      layer->set_render_layer_name (map_table["layers"][i]["render_layer"]);
      layer->set_collision (map_table["layers"][i]["collision"]);

      for (unsigned int y = 0; y < m_height; ++y)
      {
        for (unsigned int x = 1; x <= m_width; ++x)
        {
          int value      = map_table["layers"][i]["grid"][x + y * m_width][1];
          int collidable = map_table["layers"][i]["grid"][x + y * m_width][2];
          Tile tile{value};
          tile.x          = x - 1;
          tile.y          = y;
          tile.z          = map_table["layers"][i]["grid"][x + y * m_width][3];
          tile.collidable = (bool) collidable;

          // Checks surrounding tiles to set active tile edges
          // Checks tile to the top
          if (map_table["layers"][i]["grid"][(x + 1) + y * m_width] != sol::lua_nil &&
              map_table["layers"][i]["grid"][x + (y - 1) * m_width][2] == 0)
          {
            tile.active_edges.set (0);
          }
          // Checks tile to the right
          if (map_table["layers"][i]["grid"][(x + 1) + y * m_width] != sol::lua_nil &&
              map_table["layers"][i]["grid"][(x + 1) + y * m_width][2] == 0)
          {
            tile.active_edges.set (1);
          }
          // Checks tile to the bottom
          if (map_table["layers"][i]["grid"][(x + 1) + y * m_width] != sol::lua_nil &&
              map_table["layers"][i]["grid"][(x) + (y + 1) * m_width][2] == 0)
          {
            tile.active_edges.set (2);
          }
          // Checks tile to the left
          if (map_table["layers"][i]["grid"][(x + 1) + y * m_width] != sol::lua_nil &&
              map_table["layers"][i]["grid"][(x - 1) + y * m_width][2] == 0)
          {
            tile.active_edges.set (3);
          }

          layer->set_value (x - 1, y, tile);
        }
      }

      layers.emplace_back (layer);
    }

    std::cout << "Loaded TileMap: " << m_name << " from " << path << '\n';
  }

  void TileMap::load_xml (const std::string& path)
  {
    this->clear();
    std::ifstream is (path);
    cereal::XMLInputArchive archive (is);
    archive (CEREAL_NVP (m_name),
             CEREAL_NVP (m_number_of_layers),
             CEREAL_NVP (m_tile_dimension),
             CEREAL_NVP (m_width),
             CEREAL_NVP (m_height),
             CEREAL_NVP (layers));

    for (auto& layer : this->layers)
    {
      if (layer->width() != m_width || layer->height() != m_height)
      {
        layer->resize (0, m_width - layer->width(), m_height - layer->height(), 0);
      }
    }
    this->refresh();
    std::cout << "Loaded TileMap: " << m_name << " from " << path << '\n';
  }

  void TileMap::save (const std::string& path)
  {
    m_path = path;
    std::ofstream os (path);
    cereal::XMLOutputArchive archive (os);

    archive (CEREAL_NVP (m_name),
             CEREAL_NVP (m_number_of_layers),
             CEREAL_NVP (m_tile_dimension),
             CEREAL_NVP (m_width),
             CEREAL_NVP (m_height),
             CEREAL_NVP (layers));

    std::cout << "Saved TileMap: " << m_name << " in " << path << '\n';
  }

  void TileMap::update_tile (const int value, const int x, const int y, const unsigned layer_id, const bool collidable)
  {
    assert (layer_id < layers.size() && "Your layer ID is out of bounds when updating a tile.");
    assert (x < (int) m_width && "Your x coord is out of bounds when updating a tile.");
    assert (std::abs (y) < (int) m_height && "Your y coord is out of bounds when updating a tile.");
    auto layer = layers[layer_id];
    auto tile  = layer->get_value (x, y);

    tile.value      = value;
    tile.collidable = collidable;
    tile.x          = x;
    tile.y          = y;
    layers[layer_id]->set_value (x, y, tile);

    if (!m_registry.valid (tile.entity) || tile.entity == entt::null)
    {
      this->create_tile_entity (value, x, y, tile.z, layer_id);
    }
    else
    {
      if (layer_id == 0 && value == 0) {}
      // if (y == 0) this->update_tile_sprite(tile.entity, 2, layer_id);
      this->update_tile_position (tile.entity, layer_id, x, y, tile.z);
      this->update_tile_sprite (tile.entity, layer_id, value);
    }
  }

  void TileMap::update_tile_sprite (entt::entity entity, const unsigned layer_id, const int value)
  {
    if (m_registry.has<component::Sprite> (entity))
    {
      auto& spr = m_registry.get<component::Sprite> (entity);
      spr.sprite->SetDirectFrame (value);
      spr.Frame = value;
    }
    else
    {
      m_registry.emplace<component::Sprite> (entity,
                                             layers[layer_id]->get_texture_name(),
                                             glm::vec2 (m_tile_dimension, m_tile_dimension),
                                             layers[layer_id]->get_render_layer_name(),
                                             value);
    }
  }

  void
  TileMap::update_tile_position (entt::entity entity, const unsigned layer_id, const int x, const int y, const int z)
  {
    if (m_registry.has<component::Position> (entity))
    {
      auto& pos = m_registry.get<component::Position> (entity);
      pos.x     = x * m_tile_dimension;
      pos.y     = y * m_tile_dimension;
      pos.z     = z;
    }
    else
    {
      m_registry.emplace<component::Position> (entity, x * m_tile_dimension, y * m_tile_dimension, z);
    }
  }

  void TileMap::create_tile_entity (const int value, const int x, const int y, const int z, const unsigned layer_id)
  {
    auto tile = m_registry.create();
    m_registry.emplace<component::Tile> (tile, layer_id, false);
    // if (layers[layer_id]->get_texture_name() != "tileset") std::cout << layers[layer_id]->get_texture_name() << '\n';
    m_registry.emplace<component::Position> (tile, x * m_tile_dimension, y * m_tile_dimension, z);
    m_registry.emplace<component::Dimension> (tile, m_tile_dimension, m_tile_dimension);
    m_registry.emplace<component::Sprite> (tile,
                                           layers[layer_id]->get_texture_name(),
                                           glm::vec2 (m_tile_dimension, m_tile_dimension),
                                           layers[layer_id]->get_render_layer_name(),
                                           value);
    layers[layer_id]->set_entity (x, y, tile);
  }

  void TileMap::create_tile_entities (const int beginx, const int endx, const int beginy, const int endy)
  {
    // std::cout << beginx << ' ' << endx << ' ' << beginy << ' ' << endy << '\n';
    assert (beginx < endx);
    assert (beginy < endy);
    int left  = std::max (beginx / m_tile_dimension, 0);
    int right = std::min (endx / m_tile_dimension, static_cast<int> (m_width));
    int top   = std::max (beginy / m_tile_dimension, 0);
    int bottom =
        std::min (static_cast<int> (ceil (endy / static_cast<double> (m_tile_dimension))), static_cast<int> (m_height));
    int layer_counter = 0;
    // std::cout << beginx / m_tile_dimension << ' ' << right << ' ' << top << ' ' << bottom << "\n\n";

    for (const auto& layer : this->layers)
    {
      layer->set_id (layer_counter);
      for (auto y = top; y < bottom; ++y)
      {
        for (auto x = left; x < right; ++x)
        {
          const auto& layer_tile = layer->get_value (x, y);

          if (!layer_tile.visible && layer_tile.value > 0)
          {
            layer->set_visibility (x, y, true);
            this->create_tile_entity (layer_tile.value, x, y, layer_tile.z, layer_counter);
          }
          else if (!layer_tile.visible)
          {
            layer->set_visibility (x, y, true);
          }
        }
      }
      ++layer_counter;
    }
  }

  void TileMap::resize (const int top, const int right, const int bottom, const int left)
  {
    for (auto& layer : layers)
    {
      layer->resize (top, right, bottom, left);
    }
    m_width += right + left;
    m_height += top + bottom;
    this->refresh();
  }

  void TileMap::refresh()
  {
    for (auto& layer : layers)
    {
      for (unsigned j = 0; j < layer->height(); ++j)
      {
        for (unsigned i = 0; i < layer->width(); ++i)
        {
          const auto& tile = layer->get_value (i, j);
          this->update_tile (tile.value, i, j, layer->get_id(), tile.collidable);
        }
      }
    }
  }

  void TileMap::clear()
  {
    for (auto& layer : layers)
    {
      for (auto& tile : layer->m_grid)
      {
        if (m_registry.valid (tile.entity))
        {
          m_registry.destroy (tile.entity);
        }
      }
      layer->m_grid.clear();
    }
    layers.clear();
  }
} // namespace core
} // namespace stella
