#include "stella/core/tile_map.h"

namespace stella
{
namespace core
{

  TileMap::TileMap(const std::string &path)
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
    const unsigned m_number_of_layers = map_table["number_of_layers"];
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

      layers.emplace_back(layer);
    }

    std::cout << "Loaded TileMap: " << m_name << '\n';
  }

}
}

