#pragma once

#include <string>
#include "stella/core/asset.hpp"

namespace stella
{
namespace graphics
{
  enum class TextureType
  {
    DIFFUSE,
    SPECULAR,
    NORMAL,
  };

  class Texture : public core::Asset
  {
    public:
      Texture(const std::string& filepath, const TextureType type);
      ~Texture();

      void load(const std::string& filepath);
      void bind();
      void unbind();
      inline const unsigned int get_id() const { return m_id; }
      inline const int get_width() const { return m_width; }
      inline const int get_height() const { return m_height; }
      inline const TextureType get_type() const { return m_type; }

    private:
      unsigned int m_id = 0;
      const TextureType m_type;
      int m_width = 0;
      int m_height = 0;
  };

}
}
