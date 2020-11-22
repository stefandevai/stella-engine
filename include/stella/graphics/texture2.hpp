#pragma once

#include <string>
#include "stella/core/asset.hpp"

namespace stella
{
namespace graphics
{

  class Texture : public core::Asset
  {
    public:
      Texture(const std::string& filepath);
      ~Texture();

      void load(const std::string& filepath);
      void bind();
      void unbind();
      inline const unsigned int get_id() const { return m_id; }
      inline const int get_width() const { return m_width; }
      inline const int get_height() const { return m_height; }
      inline const bool has_loaded() const { return m_has_loaded; }

    private:
      unsigned int m_id = 0;
      int m_width = 0;
      int m_height = 0;
      bool m_has_loaded = false;

  };

}
}
