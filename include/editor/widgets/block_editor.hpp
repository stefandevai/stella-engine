#pragma once

#include "widget.hpp"

namespace stella
{
namespace widget
{

  class BlockEditor : public Widget
  {
    public:
      BlockEditor ();
      void render();

    private:
      int m_w = 1, m_h = 1, m_d = 1;
      int m_tiles[256][256];

      void m_reset_tiles ();
  };

} // namespace widget
} // namespace stella

