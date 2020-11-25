#pragma once

#include "widget.hpp"

namespace stella
{
namespace core
{
  class AssetManager;
}
}

namespace editor
{
namespace widget
{

  class Assets : public Widget
  {
    public:
      Assets ();
      void render(stella::core::AssetManager& asset_manager);

    private:
      void m_render_asset_list (const std::string& title, const std::vector<std::string>& names);
  };

} // namespace widget
} // namespace stella

